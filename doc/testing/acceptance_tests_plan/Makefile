TEXFLAGS:=-file-line-error -halt-on-error

MAIN:=acceptance_tests_plan.tex
PDF:=$(MAIN:.tex=.pdf)

.PHONY: all
all: pdf

#.PHONY: spell
#spell:
#	ispell -dpolish -t $F

.PHONY: pdf
pdf: $(PDF)
$(PDF): $(shell find -type f -name '*.tex')
	rm -fv "$(PDF)"
	pdflatex $(TEXFLAGS) $(MAIN)
	pdflatex $(TEXFLAGS) $(MAIN)
	pdflatex $(TEXFLAGS) $(MAIN)

.PHONY: clean
clean:
	rm -fv *.pdf *.aux *bak *.log
