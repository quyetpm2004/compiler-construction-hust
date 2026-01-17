Phạm Mạnh Quyết 20225663
Bài scanner kiểm tra trên lớp Tuần 11
Câu 1:
Sửa file 
-	token.c: sửa các keywords từ chữ hoa về chữ thường (dòng 15-37) và trong hàm keywordEq bỏ so sánh với toupper(chữ hoa) chỉ so sánh bình thường (dòng 43)
Câu 2: 
Sửa file 
-	token.h: thêm 2 enum KW_SWITCH,  KW_RETURN (dòng 41,42). Sửa KEYWORDS_COUNT = 22 (dòng 11)
-	token.c: thêm 2 giá trị cho keywords là {"switch", KW_SWITCH}, {"return", KW_RETURN}} (dòng 36,37)
-	scanner.c: in ra các trường hợp gặp keyword này: (dòng 391-395)
case KW_SWITCH:
printf("KW_SWITCH\n");
break;
case KW_RETURN:
printf("KW_RETURN\n");
Câu 3: 
Sửa file
-	charcode.h: thêm 2 charcode mới CHAR_LPAR và CHAR_RPAR (dòng 28-29)
-	charcode.c: sửa charcode thứ 91 từ CHAR_UNKNOWN thành CHAR_LSEL và thứ charcode thứ 93 tương tự (dòng 23)
-	scanner.c: thêm 2 case CHAR_LSEL, CHAR_RSEL (dòng 290-297)
Câu 4 
Sửa file
-	scanner.c: thêm else if khi của dấu < khi gặp dấu > else if (charCodes[currentChar] == CHAR_GT)
{
token->tokenType = SB_NEQ;
 readChar();
}
	(dòng 211-215)
Câu 5: 
Sửa file
-	token.h: thêm 2 type SB_PLUS_ASIGN, SB_TIMES_ASIGN (dòng 63-43)
-	scanner.c: khi đọc được dấu + mà đọc thêm được dấu = (CHAR_EQ) thì gắn tokenType là SB_PLUS_ASIGN.(dòng 168-172). Khi đọc được dấu * mà đọc thêm dấu = thì gắn tokenType là SB_TIMES_ASIGN (dòng 193-197)




Câu 6: 
Sửa file: 
-	charcode.h: thêm charcode mới CHAR_UNDERLINE 
-	charcode.c: sửa charcode 95 thành CHAR_UNDERLINE (dòng 23)
-	scanner.c: trong hàm readIdentKeyword thêm trường hợp charCodes[currentChar] == CHAR_UNDERLINE (dòng 63). Và khi length lớn hơn 10 thì bỏ qua phần còn lại cho index thứ 10 là kết thúc string (dòng 69-72)
Câu 7: 
Sửa file
-	scanner.c: khi đọc được dấu SLASH mà đọc thêm dấu SLASH nữa (dòng 184-188) thì gọi hàm CheckNewNote () để bỏ qua dòng lệnh này. Hàm CheckNewNote kiểm tra nếu là hàng mới thì chạy xong hàm này (dòng 135-146)
