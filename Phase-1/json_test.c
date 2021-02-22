#include <stdio.h>
#include <json-c/json.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE *fp;
	char buffer[1024];
	struct json_object *parsed_json;
	struct json_object *group;
    struct json_object *users;
    struct json_object *user;
    
	size_t n_users;	

	fp = fopen("server_groups.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);
    char group_names[5][20];
    int n_groups;
    lh_table* tbl = json_object_get_object(parsed_json);
    int i = 0;
    if (tbl == NULL){
            printf("tbl = null\n");
    }
    else {
        printf("size = %d\n", tbl->size);
        printf("count = %d\n", tbl->count);
        n_groups = tbl->count;
        struct lh_entry *head = tbl->head;
        if (head == NULL) printf("head is null\n");
        else{
            while (head != NULL) {
                printf("%d. %s\n", i+1, (char*)lh_entry_k(head));
                strcpy(group_names[i++], (char*)lh_entry_k(head));
                head = head->next;
            }
        }
    }
	for(i = 0; i < n_groups; i++) {
        json_object_object_get_ex(parsed_json, group_names[i], &group);
        n_users = json_object_array_length(group);
	    printf("Found %lu users in %s: \n", n_users, group_names[i]);
        for(int j = 0; j < n_users; j++) {
		    user = json_object_array_get_idx(group, j);
		    printf("%d. %s\n",j+1,json_object_get_string(user));
	    }	
    }
}