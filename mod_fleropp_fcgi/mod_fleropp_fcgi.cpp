#include <memory>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

extern "C" {
    static int fleropp_fcgi_handler(request_rec *r);
    static void fleropp_fcgi_register_hooks(apr_pool_t *p);
    /* Dispatch list for API hooks */
    module AP_MODULE_DECLARE_DATA fleropp_fcgi_module = {
        STANDARD20_MODULE_STUFF, 
        NULL,                  /* create per-dir    config structures */
        NULL,                  /* merge  per-dir    config structures */
        NULL,                  /* create per-server config structures */
        NULL,                  /* merge  per-server config structures */
        NULL,                  /* table of config file commands       */
        fleropp_fcgi_register_hooks  /* register hooks                      */
    };
}

/* The sample content handler */
static int fleropp_fcgi_handler(request_rec *r)
{
    /* Set the appropriate content type */
    ap_set_content_type(r, "text/html");

    /* Print out the IP address of the client connecting to us: */
    ap_rprintf(r, "<h2>Hello, %s!</h2>", r->useragent_ip);
    
    /* If we were reached through a GET or a POST request, be happy, else sad. */
    if ( !strcmp(r->method, "POST") || !strcmp(r->method, "GET") ) {
        ap_rputs("You used a GET or a POST method, that makes us happy!<br/>", r);
    }
    else {
        ap_rputs("You did not use POST or GET, that makes us sad :(<br/>", r);
    }

    /* Lastly, if there was a query string, let's print that too! */
    if (r->args) {
        ap_rprintf(r, "Your query string was: %s", r->args);
    }
    return OK;
}

static void fleropp_fcgi_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(fleropp_fcgi_handler, NULL, NULL, APR_HOOK_MIDDLE);
}



