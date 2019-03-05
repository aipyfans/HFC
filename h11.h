
/**
 *  简化函数注册为信号处理器的过程
 *
 * @param sig
 * @param handler
 * @return
 */
int catch_signal(int sig, void (*handler)(int));


/**
 * 创建互联网流套接字
 *
 * @return
 */
int open_listener_socket();


/**
 * 重新绑定端口
 *
 * @param socket
 * @param port
 * @return
 */
void bind_to_port(int socket, int port);


/**
 *  向客户端发送字符串
 *
 * @param socket
 * @param s
 * @return
 */
ssize_t say(int socket, char *s);


/**
 * 对 revc() 函数的封装
 * 在指定数组中保存以\0结尾的字符串
 *
 * @param socket
 * @param buf
 * @param len
 * @return
 */
ssize_t read_in(int socket, char *buf, int len);


/**
 * 如果有人在服务器运行期间按了Ctrl + C，这个函数就会赶在程序结束前关闭套接字。
 *
 * @param sig
 */
void handle_shutdown(int sig);


/**
 * 创建客户端socket链接
 * @return
 */
int open_socket(char *host, char *port);


void start_server();


void start_client(char *path);
