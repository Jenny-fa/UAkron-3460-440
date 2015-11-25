; Jennifer Yao
; 3460:440 Compiler Design
; 10/23/2015

; size_t strlen(const char *str) {
;     const char *p = str;
;     while (*p)
;         p++;
;     return p - str;
; }

define i64 @strlen(i8* %str) {
	%p = alloca i8*
	store i8* %str, i8** %p
	br label %strlen.loop.condition

strlen.loop.condition:
	%1 = load i8** %p
	%2 = load i8* %1
	%3 = icmp ne i8 %2, 0 ; *p != NULL
	br i1 %3, label %strlen.loop.body, label %strlen.return

strlen.loop.body:
	%4 = getelementptr inbounds i8* %1, i32 1
	store i8* %4, i8** %p ; p = p[1]
	br label %strlen.loop.condition

strlen.return:
	%5 = ptrtoint i8* %1 to i64 ; convert p to integer
	%6 = ptrtoint i8* %str to i64 ; convert str to integer
	%7 = sub i64 %5, %6 ; p - str
	ret i64 %7
}
