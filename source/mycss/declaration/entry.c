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

#include "mycss/declaration/entry.h"


mycss_declaration_entry_t * mycss_declaration_entry_create(mycss_declaration_t* declaration, myhtml_status_t* status)
{
    return mcobject_malloc(declaration->mcobject_entries, status);
}

void mycss_declaration_entry_clean(mycss_declaration_entry_t* entry)
{
    entry->value        = NULL;
    entry->value_type   = 0;
    entry->is_important = false;
    entry->type         = MyCSS_PROPERTY_TYPE_UNDEF;
    entry->flags        = MyCSS_DECLARATION_FLAGS_UNDEF;
}

void mycss_declaration_entry_clean_all(mycss_declaration_entry_t* entry)
{
    memset(entry, 0, sizeof(mycss_declaration_entry_t));
}

void mycss_declaration_entry_destroy(mycss_declaration_t* declaration, mycss_declaration_entry_t* dec_entry)
{
    if(dec_entry->value)
        mycss_values_destroy(declaration->ref_entry, dec_entry);
    
    mcobject_free(declaration->mcobject_entries, dec_entry);
}

void mycss_declaration_entry_append_to_current(mycss_declaration_t* declaration, mycss_declaration_entry_t* dec_entry)
{
    if(declaration->entry_last) {
        declaration->entry_last->next = dec_entry;
        dec_entry->prev = declaration->entry_last;
    }
    else {
        (*declaration->entry) = dec_entry;
    }
    
    declaration->entry_last = dec_entry;
}

void mycss_declaration_entry_remove(mycss_declaration_t* declaration, mycss_declaration_entry_t* dec_entry)
{
    if(dec_entry->next) {
        dec_entry->next->prev = dec_entry->prev;
    }
    
    if(dec_entry->prev) {
        dec_entry->prev->next = dec_entry->next;
    }
    
    dec_entry->next = NULL;
    dec_entry->prev = NULL;
}

mycss_declaration_entry_t * mycss_declaration_entry_delete(mycss_declaration_t* declaration, mycss_declaration_entry_t* dec_entry)
{
    mycss_declaration_entry_remove(declaration, dec_entry);
    mycss_declaration_entry_destroy(declaration, dec_entry);
    
    return NULL;
}

mycss_declaration_entry_t * mycss_declaration_entry(mycss_declaration_t* declaration)
{
    return declaration->entry_last;
}

mycss_declaration_entry_t * mycss_declaration_entry_last(mycss_declaration_t* declaration)
{
    return declaration->entry_last;
}

void mycss_declaration_entry_print(mycss_declaration_t* declaration, mycss_declaration_entry_t* dec_entry, FILE* fh)
{
    mycss_property_print(dec_entry->type, fh);
    fprintf(fh, ": ");
    mycss_property_value_print(dec_entry->value_type, dec_entry->value, fh);
}

void mycss_declaration_entries_print(mycss_declaration_t* declaration, mycss_declaration_entry_t* first_dec_entry, FILE* fh)
{
    while(first_dec_entry) {
        mycss_declaration_entry_print(declaration, first_dec_entry, fh);
        
        if(first_dec_entry->next) {
            fprintf(fh, "; ");
        } else {
            fprintf(fh, ";");
        }
        
        first_dec_entry = first_dec_entry->next;
    }
}

