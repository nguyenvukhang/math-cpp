run:
	cargo build
	rm -f ~/.local/state/nvim/lsp.log
	cd /home/khang/repos/math && nvim lib/real_analysis/divergence.tex
