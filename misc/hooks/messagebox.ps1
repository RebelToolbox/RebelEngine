# A generic message box.
#
# @param title    The message box's title.
# @param message  If specified, the message to be displayed.
# @param file     If specified, displays the contents of the specified file below the message.
# @param buttons  The message box's buttons separated by commas.
#                 Each button's text and the return code are separated by a colon.
# @param size     The message box's width and height separated by an 'x' e.g. "400x200".
# @param icon     The message box's icon file.

Param (
    $title = "Error",
    $message = "",
    $file = "",
    $buttons = "OK:0",
    $size = "Auto",
    $icon = "platform\windows\rebel-icon.ico"
)

Add-Type -AssemblyName System.Windows.Forms
$global:response = 0

# Create the form.
$form = New-Object System.Windows.Forms.Form
$form.Text = $title

# If icon specified and found, set the form icon.
if ([System.IO.File]::Exists($icon)) {
	$form.Icon = New-Object System.Drawing.Icon($icon)
}

# If size is specified, set the size of the form.
if ($size -ne "Auto") {
    $dimensions = $size.Split("x")
    if ($dimensions.length -eq 2) {
        $form.width = $dimensions[0]
        $form.height = $dimensions[1]
    }
}

# If a message is specified, create a label to display the message.
if ($message -ne "") {
    $label = New-Object System.Windows.Forms.Label
    $label.Text = $message
    $label.Dock = [System.Windows.Forms.DockStyle]::Top
    $form.Controls.Add($label)
}

# If a file is specified, create a TextBox to display the contents.
if ($file -ne "") {
    $textBox = New-Object System.Windows.Forms.TextBox
    # Replace unix \n line endings with Windows \r\n line endings.
    $textBox.Text = [IO.File]::ReadAllText($file).replace("`n", "`r`n")
    $textBox.Dock = [System.Windows.Forms.DockStyle]::Fill
    $textBox.Multiline = $true
    $textBox.ReadOnly = $true
    $textBox.Autosize = $true
    $textBox.Select(0, 0)
    $form.Controls.Add($textBox)
}

# If buttons are specified, add buttons.
if ($buttons -ne "") {
    # Create button container to hold the buttons.
    $buttonContainer = New-Object System.Windows.Forms.TableLayoutPanel
    $buttonContainer.Dock = [System.Windows.Forms.DockStyle]::Fill
    $buttonContainer.Autosize = $true

    # Create padding to centre the buttons
    $buttonContainerPadding = New-Object System.Windows.Forms.TableLayoutPanel
    $buttonContainerPadding.Dock = [System.Windows.Forms.DockStyle]::Bottom
    $buttonContainerPadding.Autosize = $true
    $buttonContainerPadding.ColumnCount = 3
    for ($column = 0; $column -lt 3; $column++) {
        $columnStyle = New-Object System.Windows.Forms.ColumnStyle
        $columnStyle.SizeType = [System.Windows.Forms.SizeType]::Percent
        $columnStyle.Width = 50
        $silence = $buttonContainerPadding.ColumnStyles.Add($columnStyle)
    }
    $buttonContainerPadding.ColumnStyles.Item(1).SizeType = [System.Windows.Forms.SizeType]::AutoSize

    # Add the buttons.
    $buttonsArray = $buttons.Split(",")
    $buttonContainer.ColumnCount = $buttonsArray.length
    foreach ($buttonData in $buttonsArray) {
        $buttonDataArray = $buttonData.Split(":")
        $button = New-Object System.Windows.Forms.Button
        $button.Text = $buttonDataArray[0]
        if ($buttonDataArray.length -gt 1) {
            $button.Tag = $buttonDataArray[1]
        } else {
            $button.Tag = $buttonsArray.IndexOf($buttonData)
        }
        $button.Add_Click({
            $global:response = $this.Tag
            $form.Close()
        })
        $button.Autosize = $true
        $button.Anchor = [System.Windows.Forms.AnchorStyles]::None
        $buttonContainer.Controls.Add($button)
    }
    $buttonContainerPadding.Controls.Add($buttonContainer, 1, 0)
    $form.Controls.Add($buttonContainerPadding)
}

$silence = $form.ShowDialog()
[Environment]::Exit($global:response)
