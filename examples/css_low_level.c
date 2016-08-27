/*
 Copyright (C) 2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mycss/mycss.h>

struct res_data {
    char  *data;
    size_t size;
};

struct res_data load_data_file(const char* filename)
{
    FILE *fh = fopen(filename, "rb");
    if(fh == NULL) {
        fprintf(stderr, "Can't open html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    fseek(fh, 0L, SEEK_SET);
    
    char *data = (char*)malloc(size + 1);
    if(data == NULL) {
        fprintf(stderr, "Can't allocate mem for html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    size_t nread = fread(data, 1, size, fh);
    if (nread != size) {
        fprintf(stderr, "could not read %ld bytes (%zu bytes done)\n", size, nread);
        exit(EXIT_FAILURE);
    }
    
    fclose(fh);
    
    if(size < 0)
        size = 0;
    
    struct res_data res = {data, (size_t)size};
    return res;
}

int main(int argc, const char * argv[])
{
    const char* path;
    
    if (argc == 2) {
        path = argv[1];
    }
    else {
        printf("Bad ARGV!\nUse: css_low_level <path_to_css_file>\n");
        exit(EXIT_FAILURE);
    }
    
    struct res_data res = load_data_file(path);
    
    // basic init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // current entry work init
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    // parse selectors
    status = mycss_parse(entry, MyHTML_ENCODING_UTF_8, res.data, res.size);
    
    if(status) {
        fprintf(stderr, "Parse error!\n");
        exit(EXIT_FAILURE);
    }
    
    /* print result */
    fprintf(stdout, "Result:\n");
    mycss_namespace_print_stylesheet(&entry->stylesheet->ns_stylesheet, stdout);
    mycss_stylesheet_print(entry->stylesheet, stdout);
    fprintf(stdout, "\n");
    
    // destroy all
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    free(res.data);
    
    return 0;
}


