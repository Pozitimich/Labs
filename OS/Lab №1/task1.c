#include <stdio.h>
#include <stdlib.h>

enum code_status
{
    ok,
    incorrect_input,
    overflow,
    malloc_error,
    file_opening_error,
    file_reading_error,
    file_writing_error,
    wrong_way_error
};

static const char* errors[] =
{
    "Success\n",
    "Incorrect input\n",
    "Overflow error\n",
    "Memory allocation error\n",
    "Cannot open file\n",
    "Cannot read file correctly\n",
    "Writing to file error\n",
    "Wrong way to file\n"
};
    
int main(int argc, char** argv) 
{
    if (argc != 2)
    {
        printf("Expected 2 arguments\n");
        return 1;
    }

    FILE* f;
    if ((f = fopen(argv[1], "w")) == NULL)
    {
        printf("%s", errors[wrong_way_error]);
        return 1;
    }

    unsigned char to_write[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    if (fwrite(to_write, sizeof(char), sizeof(to_write) / sizeof(char), f) != sizeof(to_write) / sizeof(char))
    {
        printf("%s", errors[file_writing_error]);
        fclose(f);
        return 1;
    }
    fclose(f);
    
    if ((f = fopen(argv[1], "rb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        return 1;
    }

    unsigned char byte;
    while (fread(&byte, sizeof(char), 1, f))
    {
        printf("%x\n", byte);
        printf("_fileno %d\n", fileno(f));
        printf("_flags %08x\n", f->_flags);
        printf("_IO_read_ptr %p\n", f->_IO_read_ptr);
        printf("_IO_read_end %p\n", f->_IO_read_end);
        printf("_IO_read_base %p\n", f->_IO_read_base);
        printf("_IO_write_base %p\n", f->_IO_write_base);
        printf("_IO_write_ptr %p\n", f->_IO_write_ptr);
        printf("_IO_write_end %p\n", f->_IO_write_end);
        printf("_IO_buf_base %p\n", f->_IO_buf_base);
        printf("_IO_buf_end %p\n", f->_IO_buf_end);
        printf("_IO_save_base %p\n", f->_IO_save_base);
        printf("_IO_backup_base %p\n", f->_IO_backup_base);
        printf("_IO_save_end %p\n", f->_IO_save_end);
        printf("_markers %p\n", f->_markers);
        printf("_chain %p\n", f->_chain);
        printf("_fileno %d\n", f->_fileno);
        printf("_flags2 %08x\n", f->_flags2);
        printf("_old_offset %ld\n", f->_old_offset);
        printf("_cur_column %d\n", f->_cur_column);
        printf("_vtable_offset %d\n", f->_vtable_offset);
        printf("_shortbuf %s\n", f->_shortbuf);
        printf("_lock %p\n", f->_lock);
        printf("_offset %ld\n", f->_offset);
        printf("_codecvt %p\n", f->_codecvt);
        printf("_wide_data %p\n", f->_wide_data);
        printf("_freeres_list %p\n", f->_freeres_list);
        printf("_freeres_buf %p\n", f->_freeres_buf);
        printf("__pad5 %ld\n", f->__pad5);
        printf("_mode %d\n", f->_mode);
        printf("_unused2 %s\n", f->_unused2);
    }
    fclose(f);

    if ((f = fopen(argv[1], "rb")) == NULL)
    {
        printf("%s", errors[file_opening_error]);
        return 1;
    }

    fseek(f, 3, SEEK_SET);
    
    unsigned char buff[4];
    
    printf("Byte buffer contains %ld bytes:\n", fread(buff, sizeof(char), 4, f));
    for (int i = 0; i < 4; ++i)
    {
        printf("%x\n", buff[i]);
    }

    fclose(f);
    return 0;
}