#include <stdlib.h>
#include <string.h>

#include <blobpack/blobpack.h>
#include <libutype/avl-cmp.h>

#include "orange_user.h"

struct orange_user_cap {
	struct avl_node avl; 
	char *name; 
}; 

struct orange_user_app {
	struct avl_node avl; 
	char *name; 
}; 

struct orange_user *orange_user_new(const char *username){
	struct orange_user *self = calloc(1, sizeof(struct orange_user)); 
	assert(self); 
	self->username = strdup(username); 
	self->avl.key = self->username; 
	avl_init(&self->acls, avl_strcmp, false, NULL); 
	return self; 
}

void orange_user_delete(struct orange_user **_self){
	struct orange_user *self = *_self; 	
	if(self->pwhash) free(self->pwhash); 
	free(self->username); 
	free(self); 
	*_self = NULL; 
}

void orange_user_set_pw_hash(struct orange_user *self, const char *pwhash){
	if(self->pwhash) free(self->pwhash); 
	self->pwhash = strdup(pwhash); 
}

void orange_user_add_acl(struct orange_user *self, const char *acl){
	char *name = NULL; 
	struct orange_user_acl *node = calloc_a(sizeof(struct orange_user_acl), &name, strlen(acl)+1); 
	node->avl.key = strcpy(name, acl); 
	avl_insert(&self->acls, &node->avl); 
}

int orange_user_from_blob_table(struct orange_user *self, struct blob_field *field){
	struct blob_field *key, *value; 	
	blob_field_for_each_kv(field, key, value){
		const char *k = blob_field_get_string(key); 
		const char *v = blob_field_get_string(value); 
		if(strcmp(k, "password") == 0){
			orange_user_set_pw_hash(self, v); 
		} else if(strcmp(k, "apps") == 0){
			struct blob_field *ch = 0; 
			blob_field_for_each_child(value, ch){
				//const char *appname = blob_field_get_string(ch); 
				//orange_user_add_app_access(self, appname, true); 
			}
		} else if(strcmp(k, "caps") == 0){
			struct blob_field *ch = 0; 
			blob_field_for_each_child(value, ch){
				//const char *capname = blob_field_get_string(ch); 
				//orange_user_add_capability(self, capname); 
			}
		}
	}
	return 0; 
}
