# get_next_line

**파일 디스크립터에서 텍스트를 한 줄씩 읽는 C 함수입니다.** `read()`로 읽은 데이터를 누적하고, 개행 뒤에 남은 문자열을 다음 호출까지 보관합니다. 42 Seoul 프로젝트로 작성했으며 파일별 읽기 상태를 정적 배열로 관리합니다.

## 핵심 구현

| 구현 | 역할 | 코드 |
|---|---|---|
| 버퍼 누적 | `BUFFER_SIZE`만큼 읽은 문자열을 기존 데이터와 결합 | [get_next_line.c](get_next_line.c) |
| 한 줄 분리 | 개행 앞을 반환하고 뒤쪽 문자열을 보관 | [get_nl](get_next_line.c) |
| FD별 상태 | `strg[OPEN_MAX]`에 파일 디스크립터별 잔여 문자열 저장 | [get_next_line.c](get_next_line.c) |
| 문자열 처리 | 길이·복제·검색·부분 문자열 함수 구현 | [get_next_line_utils.c](get_next_line_utils.c) |

## 파일 구성

| 기본 구현 | Bonus 구현 |
|---|---|
| [get_next_line.c](get_next_line.c) | [get_next_line_bonus.c](get_next_line_bonus.c) |
| [get_next_line_utils.c](get_next_line_utils.c) | [get_next_line_utils_bonus.c](get_next_line_utils_bonus.c) |
| [get_next_line.h](get_next_line.h) | [get_next_line_bonus.h](get_next_line_bonus.h) |

현재 두 구현 모두 FD별 상태 배열을 사용하며 같은 처리 로직을 갖습니다. 빌드할 때 기본 또는 Bonus 파일 한 세트를 선택합니다.

## 함수 사용법

```c
int get_next_line(int fd, char **line);
```

| 반환값 | 의미 |
|---|---|
| `1` | 개행까지 읽은 한 줄 반환. `line`에는 개행 문자 제외 |
| `0` | EOF 도달. 마지막 잔여 문자열도 `line`으로 반환 |
| `-1` | 입력 검사·읽기·읽기 버퍼 할당 단계의 오류 |

호출자는 반환받은 문자열을 `free()`합니다. `0`을 반환한 호출의 문자열도 처리·해제해야 하며, 빈 파일 또는 마지막 개행 뒤 EOF에서는 빈 문자열이 반환됩니다.

사용 예제는 아래 내용을 로컬 `main.c`로 저장합니다.

```c
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line = NULL;
    int status;

    while ((status = get_next_line(STDIN_FILENO, &line)) >= 0)
    {
        printf("%d: [%s]\n", status, line);
        free(line);
        line = NULL;
        if (status == 0)
            break;
    }
    return (status < 0);
}
```

## 빌드와 확인

C 컴파일러와 POSIX `read()`를 제공하는 환경에서 빌드합니다. 저장소 루트에서 실행합니다.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4 -D OPEN_MAX=1024 \
  main.c get_next_line.c get_next_line_utils.c -o gnl_demo
printf 'alpha\nbeta' | ./gnl_demo
```

```text
1: [alpha]
0: [beta]
```

`BUFFER_SIZE` 기본값은 4096입니다. 예제의 `4`는 한 줄이 여러 번의 읽기에 걸쳐 누적되는 동작을 확인하기 위한 값입니다.

`OPEN_MAX`를 헤더에서 제공하지 않는 환경에 맞춰 위 명령은 상태 배열 크기를 1024로 지정합니다. 이 빌드에서는 유효한 FD를 `0 ≤ fd < 1024` 범위에서 사용합니다. Bonus 빌드는 소스 두 개를 각각 `get_next_line_bonus.c`·`get_next_line_utils_bonus.c`로 바꿉니다.

## 학습 기록

기존 [get_next_line 학습 노트](https://tjung.notion.site/next_get_line-f2cd79a667554d8085705766e6a9013c)를 함께 보관합니다.
