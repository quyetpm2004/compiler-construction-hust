Làm được 3/4 câu
Phạm Mạnh Quyết 20225663
Câu 1: 
    Sửa file reader.c thêm putchar(currentChar); (dòng 19) để ghi ra output
    Sửa file error.c hàm missingToken dòng 75. Thêm dấu xuống dòng trước khi in
Câu 2: 
    Sửa file parser.c hàm compileVarDecl().  dòng 146
        eat indent trước nếu gặp dấu = thì sau đó phải là 1 biểu thức, 
        nếu gặp dấu , thì eat indent sau đó tiếp tục như trên
        cuối cùng thì eat dấu : và compileType
Câu 3: 
    sửa file parser.c dòng 459 hàm compileForSt()
        Sau for thì gặp dấu cần gặp dấu (
        Sau đó là 1 indent assign và biểu thức
        Eat dấu ;
        sau đó là 1 condition
        eat dấu ;
        sau đó eat từng cái 1 như bình thường


