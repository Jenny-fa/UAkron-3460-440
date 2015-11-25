; Jennifer Yao
; 3460:440 Compiler Design
; 10/23/2015

; extern void *memcpy(void *restrict dest, const void *restrict src, size_t n);
;
; extern size_t strlen(const char *str);
;
; char *strcpy(char *restrict dest, const char *restrict src) {
;     return memcpy(dest, src, strlen(src) + 1);
; }

declare void @llvm.memcpy.p0i8.p0i8.i64(i8*, i8*, i64, i32, i1)

declare i64 @strlen(i8*)

define i8* @strcpy(i8* noalias %dest, i8* noalias %src) {
	%1 = call i64 @strlen(i8* %src)
	%2 = add i64 %1, 1 ; strlen(src) + 1
	; use 'llvm.memcpy' instrinsic (HAHAHA NO REGRETS)
	call void @llvm.memcpy.p0i8.p0i8.i64(i8* %dest, i8* %src, i64 %2, i32 0, i1 false)
	ret i8* %dest
}
