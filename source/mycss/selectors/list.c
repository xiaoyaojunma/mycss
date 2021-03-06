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

#include "mycss/selectors/list.h"


mycss_selectors_list_t * mycss_selectors_list_create(mycss_selectors_t* selectors)
{
    mycss_selectors_list_t* selectors_list = mcobject_malloc(selectors->mcobject_list_entries, NULL);
    mycss_selectors_list_clean(selectors_list);
    return selectors_list;
}

void mycss_selectors_list_clean(mycss_selectors_list_t* selectors_list)
{
    memset(selectors_list, 0, sizeof(mycss_selectors_list_t));
}

mycss_selectors_list_t * mycss_selectors_list_destroy(mycss_selectors_t* selectors, mycss_selectors_list_t* selectors_list, bool self_destroy)
{
    if(selectors_list == NULL)
        return NULL;
    
    mycss_entry_t *entry = selectors->ref_entry;
    
    if(selectors_list->selector_list)
    {
        for(size_t i = 0; i < selectors_list->selector_list_length; i++)
            mycss_selectors_entry_destroy(entry->selectors, selectors_list->selector_list[i], true);
        
        mycss_selectors_entry_list_destroy(entry->selectors, selectors_list->selector_list);
    }
    
    if(self_destroy) {
        mcobject_free(selectors->mcobject_list_entries, selectors_list);
        return NULL;
    }
    
    return selectors_list;
}

mycss_selectors_list_t * mycss_selectors_list_append_selector(mycss_selectors_t* selectors, mycss_selectors_list_t* current_list, mycss_selectors_entry_t* selector)
{
    if(current_list->selector_list == NULL) {
        current_list->selector_list = mycss_selectors_entry_list_create(selectors);
    }
    else {
        current_list->selector_list = mycss_selectors_entry_list_add_one(selectors, current_list->selector_list, current_list->selector_list_length);
    }
    
    current_list->selector_list[current_list->selector_list_length] = selector;
    current_list->selector_list_length++;
    
    return current_list;
}

mycss_selectors_entry_t * mycss_selectors_list_last_entry(mycss_selectors_list_t* list)
{
    size_t i = list->selector_list_length;
    
    while(i) {
        i--;
        
        mycss_selectors_entry_t *entry = list->selector_list[i];
        
        while(entry) {
            if(entry->next == NULL)
                return entry;
            
            entry = entry->next;
        }
    }
    
    return NULL;
}

void mycss_selectors_list_append_to_current(mycss_selectors_t* selectors, mycss_selectors_list_t* current_list)
{
    if(selectors->list_last) {
        selectors->list_last->next = current_list;
        current_list->prev = selectors->list_last;
    }
    else {
        (*selectors->list) = current_list;
    }
    
    selectors->list_last = current_list;
}

mycss_selectors_entry_t ** mycss_selectors_list_current_chain(mycss_selectors_list_t* list)
{
    if(list->selector_list_length)
        return NULL;
    
    return &list->selector_list[ list->selector_list_length - 1 ];
}

bool mycss_selectors_list_destroy_last_empty_selector(mycss_selectors_t* selectors, mycss_selectors_list_t* list, bool destroy_found)
{
    if(list->selector_list_length == 0)
        return false;
    
    size_t idx = list->selector_list_length - 1;
    
    mycss_selectors_entry_t *entry = list->selector_list[idx];
    
    if(entry == NULL) {
        mycss_selectors_entry_destroy(selectors, entry, destroy_found);
        list->selector_list_length--;
        
        return true;
    }
    
    while(entry) {
        if(entry->next == NULL) {
            if(entry->key == NULL)
            {
                if(entry->prev)
                    entry->prev->next = NULL;
                else {
                    list->selector_list[idx] = NULL;
                    list->selector_list_length--;
                }
                
                mycss_selectors_entry_destroy(selectors, entry, destroy_found);
                
                return true;
            }
            
            return false;
        }
        
        entry = entry->next;
    }
    
    return false;
}

