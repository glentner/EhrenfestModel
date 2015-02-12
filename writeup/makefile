# build variables
odf     = --output-directory
outDIR  = out/
srcDIR  = src/
main    = main
altname = EhrenestModel
view    = open

# combo
all: pdf open

# compile latex document
pdf: 
	pdflatex ${odf} ${outDIR} ${srcDIR}${main}.tex

# view pdf document
open:
	${view} ${outDIR}${main}.pdf

# copy pdf out of bin/
copy:
	cp ${outDIR}${main}.pdf ./${altname}.pdf
