#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "User.h"
#include "HashTable.h"

void userDataDestroy(void * object) {
	UserData * userData = (UserData *)object;
	free(userData->username);
	free(userData->password);
	if (NULL != userData->group) {
		userData->group->destroy(userData->group);
	}

	free(userData);
}


UserData * initUserData(const char * username, int username_len, const char * password, int password_len, UserGroup * group) {
	UserData * userData = (UserData *)malloc(sizeof(UserData));
	bzero(userData, sizeof(UserData));
	userData->username = (char *)malloc(username_len);
	strcpy(userData->username, username);
	userData->username_len = username_len;
	
	userData->password = (char *)malloc(password_len);
	strcpy(userData->password, password);
	userData->password_len = password_len;

	userData->group = group;
	userData->destroy = userDataDestroy;
	return userData;
}


int userLogin(User * user) {

}

void userDestroy(void * object) {
	User * user = (User *)object;
	free(user->username);
	free(user->password);

	free(user);
}

User * initUser(const char * username, int username_len, const char * password, int password_len) {
	User * user = (User *)malloc(sizeof(User));
	bzero(user, sizeof(User));
	user->username = (char *)malloc(username_len);
	strcpy(user->username, username);
	user->username_len = username_len;
	
	user->password = (char *)malloc(password_len);
	strcpy(user->password, password);
	user->password_len = password_len;

	user->destroy = userDestroy;
	return user;
}

void userGroupDestroy(void * object) {
	UserGroup * group = (UserGroup *)object;
	free(group->name);
	free(group);
}

UserGroup * initUserGroup(const char * name, int name_len) {
	UserGroup * group = (UserGroup *)malloc(sizeof(UserGroup));
	group->name = (char *)malloc(name_len);
	strcpy(group->name, name);
	group->name_len = name_len;
	
	group->destroy = userGroupDestroy;
	return group;
}
