# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	./parser ../Test/test3.cmm ../Test/out.ir
testall:
	./parser ../Test/test1.cmm ../Test/out1.ir
	./parser ../Test/test2.cmm ../Test/out2.ir
	./parser ../Test/test3.cmm ../Test/out3.ir
	./parser ../Test/test4.cmm ../Test/out4.ir
testa:
	./parser ../Test/a1.c ../Test/a1_out.ir
	./parser ../Test/a2.c ../Test/a2_out.ir
	./parser ../Test/a3.c ../Test/a3_out.ir
	./parser ../Test/a4.c ../Test/a4_out.ir
	./parser ../Test/a5.c ../Test/a5_out.ir
	./parser ../Test/a6.c ../Test/a6_out.ir
	./parser ../Test/a7.c ../Test/a7_out.ir
	./parser ../Test/a8.c ../Test/a8_out.ir
	./parser ../Test/a9.c ../Test/a9_out.ir
	./parser ../Test/a10.c ../Test/a10_out.ir
	./parser ../Test/a11.c ../Test/a11_out.ir
	./parser ../Test/a12.c ../Test/a12_out.ir
	./parser ../Test/a13.c ../Test/a13_out.ir
	./parser ../Test/a14.c ../Test/a14_out.ir
	./parser ../Test/a15.c ../Test/a15_out.ir
	./parser ../Test/a16.c ../Test/a16_out.ir
	./parser ../Test/a17.c ../Test/a17_out.ir
	./parser ../Test/a18.c ../Test/a18_out.ir
	./parser ../Test/a19.c ../Test/a19_out.ir
	./parser ../Test/a20.c ../Test/a20_out.ir
testb:
	./parser ../Test/b1.c ../Test/b1_out.ir
	./parser ../Test/b2.c ../Test/b2_out.ir
testc:
	./parser ../Test/c1.c ../Test/c1_out.ir
	./parser ../Test/c2.c ../Test/c2_out.ir
testd:
	./parser ../Test/d1.c ../Test/d1_out.ir
	./parser ../Test/d2.c ../Test/d2_out.ir
	./parser ../Test/d3.c ../Test/d3_out.ir	
teste:
	./parser ../Test/e3.c ../Test/e3_out.ir
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
