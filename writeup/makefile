output_directory=--output-directory
build_location=build/

all: pdf open

pdf: ProjectGAIA_writeup.tex
	pdflatex ${output_directory} ${build_location} ProjectGAIA_writeup.tex

bib: pdf
	bibtex    ${build_location}ProjectGAIA_writeup
	pdflatex  ${output_directory} ${build_location} ProjectGAIA_writeup.tex
	pdflatex  ${output_directory} ${build_location} ProjectGAIA_writeup.tex

open:
	open  ${build_location}ProjectGAIA_writeup.pdf
