#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <errno.h>
#include <string.h>
// #include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ngx_link_func_module.h>

int is_service_on = 0;

void ngx_link_func_init_cycle(ngx_link_func_cycle_t* cycle) {
    ngx_link_func_cyc_log(info, cycle, "%s", "starting application");

    is_service_on=1;
}

void my_app_simple_get_greeting(ngx_link_func_ctx_t *ctx) {
    ngx_link_func_log_info(ctx, "Calling back and log from my_app_simple_get");

    ngx_link_func_write_resp(
        ctx,
        200,
        "200 OK",
        "text/plain",
        "greeting from ngx_link_func testing",
        sizeof("greeting from ngx_link_func testing")-1
    );
}

void my_app_simple_get_args(ngx_link_func_ctx_t *ctx) {
    ngx_link_func_log_info(ctx, "Calling back and log from my_app_simple_get_args");

    if(ctx->req_args) {
        ngx_link_func_write_resp(
            ctx,
            200,
            "200 OK",
            "text/plain",
            ctx->req_args,
            strlen(ctx->req_args)
        );
    } else {
        ngx_link_func_write_resp(
            ctx,
            204,
            "",
            "text/plain",
            NULL,
            0
        );
    }
}

void my_app_simple_get_token_args(ngx_link_func_ctx_t *ctx) {
    ngx_link_func_log_info(ctx, "Calling back and log from my_app_simple_get_token_args");

    char * tokenArgs = (char*)ngx_link_func_get_query_param(ctx, "token");
    if (! tokenArgs) {
        ngx_link_func_write_resp(
            ctx,
            401,
            "401 unauthorized",
            "text/plain",
            "Token Not Found",
            sizeof("Token Not Found")-1
        );
    } else {
        ngx_link_func_write_resp(
            ctx,
            401,
            "401 unauthorized",
            "text/plain",
            tokenArgs,
            strlen(tokenArgs)
        );
    }
}

void my_app_simple_post(ngx_link_func_ctx_t *ctx) {
    ngx_link_func_log_info(ctx, "Calling back and log from my_app_simple_post");

    ngx_link_func_write_resp(
        ctx,
        202,
        "202 Accepted and Processing",
        "text/plain",
        NULL,
        0
    );
}



void my_app_simple_get_no_resp(ngx_link_func_ctx_t *ctx) {
    ngx_link_func_log_info(ctx, "Calling back and log from my_app_simple_get_no_resp");

    //  return 404 
}

void ngx_link_func_exit_cycle(ngx_link_func_cycle_t* cyc) {
    ngx_link_func_cyc_log(info, cyc, "%s\n", "Shutting down/reloading the Application");

    is_service_on = 0;
}



#ifdef __cplusplus
}
#endif
