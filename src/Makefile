parser.tab.c parser.tab.h:	parser.y
	@bison -t -d parser.y

lex.yy.c: lexer.l parser.tab.h
	@flex lexer.l

artsy: lex.yy.c parser.tab.c parser.tab.h symbolTable.h symbolTable.c AST.c AST.h semantic.c semantic.h helper.h helper.c ircode.c ircode.h driver.c webgen.c webgen.h
	@rm -f /tmp/$(ARGS).wasm
	# Enable this command if any modifications are made to the Artsy compiler to recompile the driver
	# gcc -w -o driver parser.tab.c lex.yy.c semantic.c ircode.c driver.c AST.c symbolTable.c webgen.c
	@./driver "$(ARGS)"
	@wat2wasm /tmp/$(ARGS)_WATcode.wat -o /tmp/$(ARGS).wasm
	@rm -f /tmp/$(ARGS)_IRcode.ir /tmp/$(ARGS)_IRcodeOptimized.ir /tmp/$(ARGS)_MAINcode.wat /tmp/$(ARGS)_LOCALcode.wat /tmp/$(ARGS)_VARScode.wat /tmp/$(ARGS)_WATcode.wat

lambda-artsy: lex.yy.c parser.tab.c parser.tab.h symbolTable.h symbolTable.c AST.c AST.h semantic.c semantic.h helper.h helper.c ircode.c ircode.h driver.c webgen.c webgen.h
	gcc -static -w -o artsy parser.tab.c lex.yy.c semantic.c ircode.c driver.c AST.c symbolTable.c webgen.c