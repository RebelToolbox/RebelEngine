TEMPLATE = api.pot
POFILES = $(wildcard *.po)

all: update merge

# Update the translations template with updated API strings.
update:
	@cd ..;                                      \
	python3 tools/scripts/extract_api_strings.py \
	    --path docs modules/*/docs               \
	    --output translations/$(TEMPLATE)

# Merge changes in the translations template with each translation file.
merge:
	@echo "Merging changes."
	@for po in $(POFILES); do                                 \
	    echo "Merging changes into: $$po";                    \
	    msgmerge -w 79 -C $$po $$po $(TEMPLATE) > "$$po".new; \
	    mv -f "$$po".new $$po;                                \
	    msgattrib --output-file=$$po --no-obsolete $$po;      \
	done

check:
	@for po in $(POFILES); do        \
	    echo "Checking: $$po";       \
	    msgfmt -c $$po -o /dev/null; \
	done
