PYTHON ?= python3
LATEXCMD = pdflatex -halt-on-error -interaction=nonstopmode -shell-escape -output-directory build/
export TEXINPUTS=.:content/tex/:
export max_print_line = 1048576

.PHONY: help fast kactl test test-compiles test-usage verify-pdf clean
help:
	@echo "Better Call Solve — MIUP team notebook"
	@echo "make kactl        Build the PDF (three passes for a stable contents page)"
	@echo "make fast         Build one draft pass"
	@echo "make test         Run all release and sanitizer batteries"
	@echo "make test-compiles Compile all printed headers alone and together"
	@echo "make test-usage   Check literal printed usage snippets"
	@echo "make verify-pdf   Check page count and PDF text bounds"
	@echo "make clean        Remove generated build files"

build:
	mkdir -p build

fast: | build
	$(LATEXCMD) content/kactl.tex
	cp build/kactl.pdf kactl.pdf

kactl: | build
	$(LATEXCMD) content/kactl.tex
	$(LATEXCMD) content/kactl.tex
	$(LATEXCMD) content/kactl.tex
	cp build/kactl.pdf kactl.pdf

test:
	$(PYTHON) tests/run.py $(TEST_ARGS)

test-compiles:
	$(PYTHON) tests/compile.py

test-usage:
	$(PYTHON) tests/usage_examples.py
	$(PYTHON) tests/usage_examples.py --sanitize

verify-pdf:
	$(PYTHON) tests/pdf.py kactl.pdf

clean:
	rm -rf build
