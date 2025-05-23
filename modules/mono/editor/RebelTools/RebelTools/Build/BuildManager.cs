using System;
using System.IO;
using System.Threading.Tasks;
using RebelTools.Ides.Rider;
using RebelTools.Internals;
using JetBrains.Annotations;
using static RebelTools.Internals.Globals;
using File = RebelTools.Utils.File;
using OS = RebelTools.Utils.OS;
using Path = System.IO.Path;

namespace RebelTools.Build
{
    public static class BuildManager
    {
        private static BuildInfo _buildInProgress;

        public const string PropNameMSBuildMono = "MSBuild (Mono)";
        public const string PropNameMSBuildVs = "MSBuild (VS Build Tools)";
        public const string PropNameMSBuildJetBrains = "MSBuild (JetBrains Rider)";
        public const string PropNameDotnetCli = "dotnet CLI";

        public const string MsBuildIssuesFileName = "msbuild_issues.csv";
        public const string MsBuildLogFileName = "msbuild_log.txt";

        public delegate void BuildLaunchFailedEventHandler(BuildInfo buildInfo, string reason);

        public static event BuildLaunchFailedEventHandler BuildLaunchFailed;
        public static event Action<BuildInfo> BuildStarted;
        public static event Action<BuildResult> BuildFinished;
        public static event Action<string> StdOutputReceived;
        public static event Action<string> StdErrorReceived;

        private static void RemoveOldIssuesFile(BuildInfo buildInfo)
        {
            string issuesFile = GetIssuesFilePath(buildInfo);

            if (!File.Exists(issuesFile))
                return;

            File.Delete(issuesFile);
        }

        private static void ShowBuildErrorDialog(string message)
        {
            var plugin = RebelSharpEditor.Instance;
            plugin.ShowErrorDialog(message, "Build error");
            plugin.MakeBottomPanelItemVisible(plugin.MSBuildPanel);
        }

        public static void RestartBuild(BuildOutputView buildOutputView) => throw new NotImplementedException();
        public static void StopBuild(BuildOutputView buildOutputView) => throw new NotImplementedException();

        private static string GetLogFilePath(BuildInfo buildInfo)
        {
            return Path.Combine(buildInfo.LogsDirPath, MsBuildLogFileName);
        }

        private static string GetIssuesFilePath(BuildInfo buildInfo)
        {
            return Path.Combine(buildInfo.LogsDirPath, MsBuildIssuesFileName);
        }

        private static void PrintVerbose(string text)
        {
            if (Rebel.OS.IsStdoutVerbose())
                Rebel.GD.Print(text);
        }

        public static bool Build(BuildInfo buildInfo)
        {
            if (_buildInProgress != null)
                throw new InvalidOperationException("A build is already in progress");

            _buildInProgress = buildInfo;

            try
            {
                BuildStarted?.Invoke(buildInfo);

                // Required in order to update the build tasks list
                Internal.RebelMainIteration();

                try
                {
                    RemoveOldIssuesFile(buildInfo);
                }
                catch (IOException e)
                {
                    BuildLaunchFailed?.Invoke(buildInfo, $"Cannot remove issues file: {GetIssuesFilePath(buildInfo)}");
                    Console.Error.WriteLine(e);
                }

                try
                {
                    int exitCode = BuildSystem.Build(buildInfo, StdOutputReceived, StdErrorReceived);

                    if (exitCode != 0)
                        PrintVerbose($"MSBuild exited with code: {exitCode}. Log file: {GetLogFilePath(buildInfo)}");

                    BuildFinished?.Invoke(exitCode == 0 ? BuildResult.Success : BuildResult.Error);

                    return exitCode == 0;
                }
                catch (Exception e)
                {
                    BuildLaunchFailed?.Invoke(buildInfo, $"The build method threw an exception.\n{e.GetType().FullName}: {e.Message}");
                    Console.Error.WriteLine(e);
                    return false;
                }
            }
            finally
            {
                _buildInProgress = null;
            }
        }

        public static async Task<bool> BuildAsync(BuildInfo buildInfo)
        {
            if (_buildInProgress != null)
                throw new InvalidOperationException("A build is already in progress");

            _buildInProgress = buildInfo;

            try
            {
                BuildStarted?.Invoke(buildInfo);

                try
                {
                    RemoveOldIssuesFile(buildInfo);
                }
                catch (IOException e)
                {
                    BuildLaunchFailed?.Invoke(buildInfo, $"Cannot remove issues file: {GetIssuesFilePath(buildInfo)}");
                    Console.Error.WriteLine(e);
                }

                try
                {
                    int exitCode = await BuildSystem.BuildAsync(buildInfo, StdOutputReceived, StdErrorReceived);

                    if (exitCode != 0)
                        PrintVerbose($"MSBuild exited with code: {exitCode}. Log file: {GetLogFilePath(buildInfo)}");

                    BuildFinished?.Invoke(exitCode == 0 ? BuildResult.Success : BuildResult.Error);

                    return exitCode == 0;
                }
                catch (Exception e)
                {
                    BuildLaunchFailed?.Invoke(buildInfo, $"The build method threw an exception.\n{e.GetType().FullName}: {e.Message}");
                    Console.Error.WriteLine(e);
                    return false;
                }
            }
            finally
            {
                _buildInProgress = null;
            }
        }

        public static bool BuildProjectBlocking(string config, [CanBeNull] string[] targets = null, [CanBeNull] string platform = null)
        {
            var buildInfo = new BuildInfo(RebelSharpDirs.ProjectSlnPath, targets ?? new[] {"Build"}, config, restore: true);

            // If a platform was not specified, try determining the current one. If that fails, let MSBuild auto-detect it.
            if (platform != null || OS.PlatformNameMap.TryGetValue(Rebel.OS.GetName(), out platform))
                buildInfo.CustomProperties.Add($"RebelTargetPlatform={platform}");

            if (Internal.RebelIsRealTDouble())
                buildInfo.CustomProperties.Add("RebelRealTIsDouble=true");

            return BuildProjectBlocking(buildInfo);
        }

        private static bool BuildProjectBlocking(BuildInfo buildInfo)
        {
            if (!File.Exists(buildInfo.Solution))
                return true; // No solution to build

            // Make sure the API assemblies are up to date before building the project.
            // We may not have had the chance to update the release API assemblies, and the debug ones
            // may have been deleted by the user at some point after they were loaded by the Rebel editor.
            string apiAssembliesUpdateError = Internal.UpdateApiAssembliesFromPrebuilt(buildInfo.Configuration == "ExportRelease" ? "Release" : "Debug");

            if (!string.IsNullOrEmpty(apiAssembliesUpdateError))
            {
                ShowBuildErrorDialog("Failed to update the Rebel API assemblies");
                return false;
            }

            using (var pr = new EditorProgress("mono_project_debug_build", "Building project solution...", 1))
            {
                pr.Step("Building project solution", 0);

                if (!Build(buildInfo))
                {
                    ShowBuildErrorDialog("Failed to build project solution");
                    return false;
                }
            }

            return true;
        }

        public static bool EditorBuildCallback()
        {
            if (!File.Exists(RebelSharpDirs.ProjectSlnPath))
                return true; // No solution to build

            GenerateEditorScriptMetadata();

            if (RebelSharpEditor.Instance.SkipBuildBeforePlaying)
                return true; // Requested play from an external editor/IDE which already built the project

            return BuildProjectBlocking("Debug");
        }

        // NOTE: This will be replaced with C# source generators in 4.0
        public static void GenerateEditorScriptMetadata()
        {
            string editorScriptsMetadataPath = Path.Combine(RebelSharpDirs.ResMetadataDir, "scripts_metadata.editor");
            string playerScriptsMetadataPath = Path.Combine(RebelSharpDirs.ResMetadataDir, "scripts_metadata.editor_player");

            CsProjOperations.GenerateScriptsMetadata(RebelSharpDirs.ProjectCsProjPath, editorScriptsMetadataPath);

            if (!File.Exists(editorScriptsMetadataPath))
                return;

            try
            {
                File.Copy(editorScriptsMetadataPath, playerScriptsMetadataPath);
            }
            catch (IOException e)
            {
                throw new IOException("Failed to copy scripts metadata file.", innerException: e);
            }
        }

        // NOTE: This will be replaced with C# source generators in 4.0
        public static string GenerateExportedGameScriptMetadata(bool isDebug)
        {
            string scriptsMetadataPath = Path.Combine(RebelSharpDirs.ResMetadataDir, $"scripts_metadata.{(isDebug ? "debug" : "release")}");
            CsProjOperations.GenerateScriptsMetadata(RebelSharpDirs.ProjectCsProjPath, scriptsMetadataPath);
            return scriptsMetadataPath;
        }

        public static void Initialize()
        {
            // Build tool settings
            var editorSettings = RebelSharpEditor.Instance.GetEditorInterface().GetEditorSettings();

            BuildTool msbuildDefault;

            if (OS.IsWindows)
            {
                if (RiderPathManager.IsExternalEditorSetToRider(editorSettings))
                    msbuildDefault = BuildTool.JetBrainsMsBuild;
                else
                    msbuildDefault = !string.IsNullOrEmpty(OS.PathWhich("dotnet")) ? BuildTool.DotnetCli : BuildTool.MsBuildVs;
            }
            else
            {
                msbuildDefault = !string.IsNullOrEmpty(OS.PathWhich("dotnet")) ? BuildTool.DotnetCli : BuildTool.MsBuildMono;
            }

            EditorDef("mono/builds/build_tool", msbuildDefault);

            string hintString;

            if (OS.IsWindows)
            {
                hintString = $"{PropNameMSBuildMono}:{(int)BuildTool.MsBuildMono}," +
                             $"{PropNameMSBuildVs}:{(int)BuildTool.MsBuildVs}," +
                             $"{PropNameMSBuildJetBrains}:{(int)BuildTool.JetBrainsMsBuild}," +
                             $"{PropNameDotnetCli}:{(int)BuildTool.DotnetCli}";
            }
            else
            {
                hintString = $"{PropNameMSBuildMono}:{(int)BuildTool.MsBuildMono}," +
                             $"{PropNameDotnetCli}:{(int)BuildTool.DotnetCli}";
            }

            editorSettings.AddPropertyInfo(new Rebel.Collections.Dictionary
            {
                ["type"] = Rebel.Variant.Type.Int,
                ["name"] = "mono/builds/build_tool",
                ["hint"] = Rebel.PropertyHint.Enum,
                ["hint_string"] = hintString
            });
        }
    }
}
