#include <stdlib.h>
typedef struct {
    char *hashed_value;
    size_t hash_length;
    char *salt;
    size_t salt_length;
} password;

int verify_screen(const char *passwd, const size_t p_length, const char *id, const size_t id_len);
int generate_access_token(const char *id, const size_t id_len, char *token, size_t *length);
int verify_access_token(const char *id, const size_t id_len, const char *token, const size_t *length);
