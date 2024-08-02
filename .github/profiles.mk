PROFILE := MA2108

# Profile names
PROFILE_NAME_ST2131 := Statistics
PROFILE_NAME_MA2108 := Real Analysis
PROFILE_NAME_MA2101 := Linear Algebra
PROFILE_NAME_MA2213 := Numerical Analysis

# use git branch to select profile
GIT_BRANCH_st := ST2131
GIT_BRANCH_la := MA2101
GIT_BRANCH_na := MA2213

# MA2108: Real Analysis
TEXTBOOKS_MA2108 += ~/repos/pdfs/MA2108.pdf

# MA2213: Numerical Analysis
# TEXTBOOKS_MA2213 += ~/repos/pdfs/numerical-analysis/MA2213_4.pdf
# TEXTBOOKS_MA2213 += ~/repos/pdfs/LinearAlgebraDoneRight.pdf
# TEXTBOOKS_MA2213 += ~/uni/MA2213/Interpolation.pdf
# TEXTBOOKS_MA2213 += ~/uni/MA2213/Practical_Interpolation.pdf
# TEXTBOOKS_MA2213 += ~/uni/MA2213/Orthogonal_Polynomials_and_Piecewise_Interpolation.pdf
# TEXTBOOKS_MA2213 += ~/uni/MA2213/Numerical_Integration.pdf
TEXTBOOKS_MA2213 += ~/uni/MA2213/Errors_Composite_Quadrature.pdf
# TEXTBOOKS_MA2213 += ~/uni/MA2213/Numerical_Differentiation.pdf

# ST2131: Statistics
TEXTBOOKS_ST2131 += ~/uni/ST2131/textbook-ST2131.pdf
TEXTBOOKS_ST2131 += ~/uni/ST2131/L04.pdf

# MA2101: Linear Algebra II
TEXTBOOKS_MA2101 += ~/repos/pdfs/LinearAlgebraDoneRight.pdf

# ────────────────────────────────────────────────────────────────────

# get current git branch
GIT_BRANCH := $(shell git branch --show-current)

# get PROFILE
ifneq (${GIT_BRANCH_$(GIT_BRANCH)},)
PROFILE := ${GIT_BRANCH_$(GIT_BRANCH)}
endif

# get PROFILE derivatives
PROFILE_NAME  := ${PROFILE_NAME_$(PROFILE)}
TEXTBOOKS     := ${TEXTBOOKS_$(PROFILE)}
