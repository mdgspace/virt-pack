#include <stdio.h>
#include <openssl/opensslv.h>
#include <curl/curl.h>

int main()
{
    // Print OpenSSL version
    printf("OpenSSL version: %s\n", OPENSSL_VERSION_TEXT);

    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (curl)
    {
        printf("libcurl initialized successfully.\n");
        curl_easy_cleanup(curl);
    }
    else
    {
        fprintf(stderr, "Failed to initialize libcurl.\n");
        return 1;
    }

    return 0;
}
