src_dir := "src"
srcs := src_dir / "sblvm.c " + src_dir / "sblcc.c " + src_dir / "sblinst.c " + src_dir / "sblconst.c " + src_dir / "sblstack.c " + src_dir / "sblop.c "

sbl_src  := src_dir / "sbl.c "  + srcs
sblc_src := src_dir / "sblc.c " + srcs
sbli_src := src_dir / "sbli.c " + srcs
sblr_src := src_dir / "sblr.c " + srcs

all: sbl sblc sbli sblr

sbl:
    cc {{sbl_src}}  -o sbl
sblc:
    cc {{sblc_src}} -o sblc
sbli:
    cc {{sbli_src}} -o sbli
sblr:
    cc {{sblr_src}} -o sblr
