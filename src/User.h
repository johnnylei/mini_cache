#ifndef __USER_H__
#define __USER_H__
typedef struct _userGroup {
	char * name;
	int name_len;
	void (* destroy)(void *);
} UserGroup;

typedef struct _userData {
	char * username;
	int username_len;
	char * password;
	int password_len;
	UserGroup * group;

	void (* destroy)(void *);
} UserData;

typedef struct _user {
	char * username;
	int username_len;
	char * password;
	int password_len;

	int (* login)(struct _user *);
	void (* destroy)(void *);
} User;

UserGroup * initUserGroup(const char * name, int name_len);
UserData * initUserData(const char * username, int username_len, const char * password, int password_len, UserGroup * group);
User * initUser(const char * username, int username_len, const char * password, int password_len);
#endif
