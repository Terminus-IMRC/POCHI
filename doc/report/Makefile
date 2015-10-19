TMPDIR := tmp
TMPIMGDIR := $(TMPDIR)/img
OUTDIR := $(TMPDIR)

TARGETS_DVI := $(OUTDIR)/main.dvi
TARGETS_PDF := $(OUTDIR)/main.pdf
TARGETS := $(TARGETS_DVI) $(TARGETS_PDF)

SRCS_MAIN := main.tex
SRCS := $(SRCS_MAIN)
SRCS += header.tex
SRCS += $(addprefix ./, main.tex $(addprefix img/, circuit.png motion.png peace.png perspective.png tilt.png wire-left.png wire-right.png wire-with-motor.png))
ALLDEPS := Makefile

LATEXMK := latexmk
GROFF := groff -p
DVIPDF := dvipdfmx
PDFTOCAIRO := pdftocairo
TOUCH := touch
MKDIR := mkdir -p
CP := cp -a
RM := rm -f
RMDIR := rmdir

# $(eval $(call set-roff-rule-core, roff-file-name, output-dir-name, eps-file-name, pdf-file-name, dvi-file-name))
define set-roff-rule-core
 OBJS_PDF_EPS += $3
 OBJS_PDF += $4
 OBJS_DVI += $5
 $(TARGETS): $3
 $3: $4 | $2
	$(PDFTOCAIRO) -eps $$< $$@
 $4: $5 | $2
	$(DVIPDF) -o $$@ $$<
 $5: $1 | $2
	$(GROFF) -Tdvi $$< >$$@
endef # set-roff-rule-core

# $(eval $(call set-roff-rule, roff-file-name))
define set-roff-rule
 $(eval $(call set-roff-rule-core, $1, $(TMPIMGDIR), \
  $$(addprefix $(TMPIMGDIR)/, $(patsubst %.roff, %.roff.dvi.pdf.eps, $(notdir $1))), \
  $$(addprefix $(TMPIMGDIR)/, $(patsubst %.roff, %.roff.dvi.pdf, $(notdir $1))), \
  $$(addprefix $(TMPIMGDIR)/, $(patsubst %.roff, %.roff.dvi, $(notdir $1)))))
endef # set-roff-rule

SRCS_ROFF := $(filter %.roff, $(SRCS))
$(foreach SRC_ROFF, $(SRCS_ROFF), $(eval $(call set-roff-rule, $(SRC_ROFF))))

all: dvi

.PHONY: dvi
dvi: $(TARGETS_DVI)

.PHONY: pdf
pdf: $(TARGETS_PDF)

$(TARGETS_DVI): $(SRCS) $(ALLDEPS) | $(TMPDIR)
	TMPDIR="$(TMPDIR)" $(LATEXMK) -dvi -pdf- -outdir="$(TMPDIR)" -auxdir="$(TMPDIR)" $(SRCS_MAIN)
ifneq '$(TMPDIR)' '$(OUTDIR)'
	$(CP) "$(addprefix $(TMPDIR)/, $(notdir $@))" "$@"
else
	$(TOUCH) "$@"
endif

$(TARGETS_PDF): $(SRCS) $(ALLDEPS) | $(TMPDIR)
	TMPDIR="$(TMPDIR)" $(LATEXMK) -pdfdvi -outdir="$(TMPDIR)" -auxdir="$(TMPDIR)" $(SRCS_MAIN)
ifneq '$(TMPDIR)' '$(OUTDIR)'
	$(CP) "$(addprefix $(TMPDIR)/, $(notdir $@))" "$@"
else
	$(TOUCH) "$@"
endif

ifneq '$(TMPDIR)' '$(OUTDIR)'
$(OUTDIR):
	$(MKDIR) "$@"
endif

.PHONY: $(TMPDIR)
$(TMPDIR):
	scripts/mount_tmp.sh

$(TMPIMGDIR): | $(TMPDIR)
	$(MKDIR) "$@"

.PHONY: clean
clean:
	TMPDIR="$(TMPDIR)" $(LATEXMK) -dvi -pdf- -outdir="$(TMPDIR)" -auxdir="$(TMPDIR)" $(SRCS_MAIN) -C
	TMPDIR="$(TMPDIR)" $(LATEXMK) -pdfdvi -outdir="$(TMPDIR)" -auxdir="$(TMPDIR)" $(SRCS_MAIN) -C
	$(RM) $(TARGETS)
	$(RM) "$(TMPDIR)"/dvipdfm*
	$(RM) "$(TMPDIR)"/gs_*
	$(RM) $(OBJS_PDF_EPS)
	$(RM) $(OBJS_PDF)
	$(RM) $(OBJS_DVI)
	$(RMDIR) $(TMPIMGDIR)

.PHONY: print-srcs
print-srcs:
	@echo "$(SRCS)"
