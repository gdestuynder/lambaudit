HANDLER		:= test.handler
HANDLER_CODE	:= test.sh
LAMBDA_ROLE	:= arn:aws:iam::656532927350:role/lambda_basic_execution
FUNCTION_NAME	:= sandboxed-lambda

.PHONY: all package
all: package
package: compile
	zip function.zip bootstrap real_bootstrap lambaudit.so test.sh

.PHONY: create
create:
	aws lambda create-function --function-name $(FUNCTION_NAME) --zip-file fileb://function.zip --handler $(HANDLER) --runtime provided --role $(LAMBDA_ROLE)

.PHONY: update
update:
	aws lambda update-function-code --zip-file fileb://function.zip --function-name $(FUNCTION_NAME)

.PHONY: remove
remove:
	aws lambda delete-function --function-name $(FUNCTION_NAME)

.PHONY: invoke
invoke:
	aws lambda invoke --function-name $(FUNCTION_NAME)  --payload '{}' /dev/stdout


compile: lambaudit.c
	gcc -fPIC -shared  -o lambaudit.so lambaudit.c -ldl

.PHONY: clean
clean:
	rm function.zip
	rm *so
