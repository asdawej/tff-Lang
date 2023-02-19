// This header is for some assistant functions.


char forechar(FILE* rfp){
    fpos_t temp;
    fgetpos(rfp, &temp);
    temp -= 2;
    fsetpos(rfp, &temp);
    char ret = fgetc(rfp);
    fgetc(rfp);
    return ret;
}
