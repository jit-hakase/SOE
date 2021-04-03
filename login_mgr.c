#include <stdio.h>
#include <stdlib.h>

/* stat */
enum {
	CONNECT,
	LOGIN,
	TRADE
};

/* action */
enum {
	REQ_USER_NULL,
	REQ_USER_DISCONNECT,
	REQ_USER_CONNECT,
	REQ_USER_LOGIN,
	REQ_USER_TRADE,
	REQ_QUERY_DATA,
	REQ_ORDER_INSERT,
	REQ_ORDER_ACTION
};

struct stat_t;

struct action_fn_t
{
	void (*connect)(struct stat_t *);
	void (*login)(struct stat_t *);
	void (*query)(struct stat_t *);
	void (*trade)(struct stat_t *);
	void (*req_order_insert)(struct stat_t *);
	void (*req_order_action)(struct stat_t *);
	void (*disconnect)(struct stat_t *);
};

struct stat_t {
	int fd;
	int stat;
	int act;
	void (*fn)(struct stat_t *stat, struct action_fn_t *act_fn);
};

void stat_null(struct stat_t *stat, struct action_fn_t *act_fn);
void stat_connect(struct stat_t *stat, struct action_fn_t *act_fn);
void stat_login(struct stat_t *stat, struct action_fn_t *act_fn);
void stat_trade(struct stat_t *stat, struct action_fn_t *act_fn);

void stat_null(struct stat_t *stat, struct action_fn_t *act_fn)
{
	if (stat->act == REQ_USER_CONNECT) {
		act_fn->connect(stat);
	}
}

void stat_connect(struct stat_t *stat, struct action_fn_t *act_fn)
{
	if (stat->act == REQ_USER_LOGIN) {
		act_fn->login(stat);
	} else if (stat->act == REQ_USER_DISCONNECT) {
		act_fn->disconnect(stat);
	}
}

void stat_login(struct stat_t *stat, struct action_fn_t *act_fn)
{
	if (stat->act == REQ_USER_TRADE) {
		act_fn->trade(stat);
	} if (stat->act == REQ_QUERY_DATA) {
		act_fn->query(stat);
	} else if (stat->act == REQ_USER_DISCONNECT) {
		act_fn->disconnect(stat);
	}
}

void stat_trade(struct stat_t *stat, struct action_fn_t *act_fn)
{
	if (stat->act == REQ_ORDER_INSERT) {
		act_fn->req_order_insert(stat);
	} else if (stat->act == REQ_ORDER_ACTION) {
		act_fn->req_order_action(stat);
	} else if (stat->act == REQ_QUERY_DATA) {
		act_fn->query(stat);
	} else if (stat->act == REQ_USER_DISCONNECT) {
		act_fn->disconnect(stat);
	}
}

void action_connect(struct stat_t *stat)
{
	printf("connect\n");
	stat->stat = CONNECT;
	stat->fn = stat_connect;
}

void action_login(struct stat_t *stat)
{
	printf("login\n");
	stat->stat = LOGIN;
	stat->fn = stat_login;
}

void action_trade(struct stat_t *stat)
{
	printf("trade\n");
	stat->stat = TRADE;
	stat->fn = stat_trade;
}

void action_disconnect(struct stat_t *stat)
{
	printf("disconnect\n");
	stat->stat = 0;
	stat->fn = stat_null;
}

void action_query(struct stat_t *stat)
{
	printf("query\n");
}

void req_order_insert(struct stat_t *stat)
{
	printf("req_order_insert\n");
}

void req_order_action(struct stat_t *stat)
{
	printf("req_order_action\n");
}

int get_action(char ch)
{
	switch (ch) {
		case 'd' : return REQ_USER_DISCONNECT;
		case 'c' : return REQ_USER_CONNECT;
		case 'l' : return REQ_USER_LOGIN;
		case 't' : return REQ_USER_TRADE;
		case 'i' : return REQ_ORDER_INSERT;
		case 'a' : return REQ_ORDER_ACTION;
		case 'q' : return REQ_QUERY_DATA;
		default  : return REQ_USER_DISCONNECT;
	}
	return REQ_USER_DISCONNECT;
}

int main()
{
	struct action_fn_t *act_fn;
	struct stat_t *stat;
	
	act_fn = malloc(sizeof(struct action_fn_t));
	act_fn->connect = action_connect;
	act_fn->login = action_login;
	act_fn->trade = action_trade;
	act_fn->query = action_query;
	act_fn->req_order_insert = req_order_insert;
	act_fn->req_order_action = req_order_action;
	act_fn->disconnect = action_disconnect;
	
	stat = malloc(sizeof(struct stat_t));
	stat->fn = stat_null;
	stat->stat = 0;
	stat->act = REQ_USER_NULL;
	
	for (;;) {
		char ch = getchar();
		if (ch == '\n') continue;
		stat->act = get_action(ch);
		stat->fn(stat, act_fn);
	}
	return 0;
}
