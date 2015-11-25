; Jennifer Yao
; 3460:440 Compiler Design
; 10/23/2015

; int strcmp(const char *str1, const char *str2) {
;     const unsigned char *p1 = (const unsigned char *)str1;
;     const unsigned char *p2 = (const unsigned char *)str2;
;     unsigned char c1, c2;
;
;     do {
;         c1 = (unsigned char)*p1++;
;         c2 = (unsigned char)*p2++;
;         if (c1 == '\0')
;             break;
;     }
;     while (c1 == c2);
;
;     return c1 - c2;
; }

define i32 @strcmp(i8* %str1, i8* %str2) {
	%p1 = alloca i8*
	%p2 = alloca i8*
	%c1 = alloca i8
	%c2 = alloca i8
	store i8* %str1, i8** %p1
	store i8* %str2, i8** %p2
	br label %strcmp.loop.body

strcmp.loop.body:
	%1 = load i8** %p1
	%2 = load i8* %1
	store i8 %2, i8* %c1 ; c1 = *p1
	%3 = getelementptr inbounds i8* %1, i32 1
	store i8* %3, i8** %p1 ; p1 = p1[1]
	%4 = load i8** %p2
	%5 = load i8* %4
	store i8 %5, i8* %c2 ; c2 = *p2
	%6 = getelementptr inbounds i8* %4, i32 1
	store i8* %6, i8** %p2 ; p2 = p2[1]
	%7 = load i8* %c1
	%8 = load i8* %c2
	%9 = icmp eq i8 %7, 0 ; c1 == '\0'
	br i1 %9, label %strcmp.return, label %strcmp.loop.condition

strcmp.loop.condition:
	%10 = icmp eq i8 %7, %8 ; c1 == c2
	br i1 %10, label %strcmp.loop.body, label %strcmp.return

strcmp.return:
	%11 = zext i8 %7 to i32
	%12 = zext i8 %8 to i32
	%13 = sub nsw i32 %11, %12 ; c1 - c2
	ret i32 %13
}
