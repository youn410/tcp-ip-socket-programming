#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void DieWithError(char *errorMessage);
void InterruptSignalHandler(int signalType); /* 割り込みシグナル処理関数 */

int main(int argc, char *argv[]) {
  /**
   * struct sigaction {
   *   void (*sa_handler)(int); シグナルハンドラ
   *   sigset_t sa_mask; ハンドラ実行中にブロックされるシグナル
   *   int sa_flags; デフォルト動作を変更するためのフラグ
   * }
   */
  struct sigaction handler; /* シグナルハンドラを指定する構造体 */

  /* InterruptSignalHandler()をシグナルハンドラとして設定 */
  handler.sa_handler = InterruptSignalHandler;
  /* 全シグナルをマスクするマスクを作成 */
  if (sigfillset(&handler.sa_mask) < 0)
    DieWithError("sigfillset() failed");
  /* フラグなし */
  handler.sa_flags = 0;

  /* 割り込みシグナルに対する処理を設定 */
  if (sigaction(SIGINT, &handler, 0) < 0)
    DieWithError("sigaction() failed");

  for(;;)
    pause();

  exit(0);
}

void InterruptSignalHandler(int signalType) {
  printf("Interrupt Received. Existing program.\n");
  exit(1);
  // sleep(5);
}
