1. 컴파일 방법 : make 명령어로 컴파일을 실시한다.
2. 실행방법
	1. 새로운 파일로 실행하는 방법: ./viva로 실행시 새로운 파일로 실행할 수 있다.
	2. 기존의 있는 파일로 실행: ./viva <FILE_NAME>을 이용하여 기존의 파일을 열 수 있다.

3. 텍스트 수정: vi와 달리 입력 모드로 전환하는 과정없이 바로 텍스트 입력, 삭제, 수정이 가능하다.

4. 이동
	1. 화살표키: 기본적으로 커서을 위, 아래, 좌, 우로 이동 할수 있다. 단, 문자열의 길이를 넘어서는 커서를 이동할 수 없다.
	2. 홈키: 홈키를 입력하면 커서는 문자열의 맨 앞으로 이동한다.
	3. 엔드키: 엔드키를 입력하면 커서는 문자열의 맨 뒤로 이동한다.
	4. 페이지 업: 페이지 업 키를 누르면 화면의 크기 만큼 위로 이동한다.
	5. 페이지 다운 : 페이지 다운 키를 누르면 화면의 크기 만큼 아래로 이동한다.

5. 저장
	1. 저장은 ctrl+s로 저장을 실시한다.
	2. 기존의 파일을 연 경우: ctrl+s 누르면 파일 이름을 입력하는 과정 없이 저장된다.
	3. 새로운 파일을 연 경우: ctrl+s 누르면 메시지바가 파일 이름 입력바로 바뀌고 파일 이름을 입력하면 저장된다.

6. 탐색
	1. 탐색은 ctrl+F로 시작한다.
	2. 탐색을 시작하면 메시지바에 검색할 문자열을 입력한다.
	3. 문자열을 입력하고 엔터를 입력하면, 검색을 시작하고 match된 문자들은 하이라이팅 된다.
	4. 하이라이팅 된 후 엔터를 입력하면 그 위치에서 바로 수정이 가능하다.
	5. 하이라이팅 된 이후 ESC를 누루면 검색을 종료하고, 커서는 검색 전 위치로 돌아간다.
	6. 화살표 위, 아래 키를 이용해서 이전 검색 문자, 이후 검색 문자로 이동 할 수 있다.

7. 나가기: Ctrl + q를 이용해서 나갈 수 있다.
	1. 저장 되지 않은 상태에서 Ctrl + q를 한 번 누르면, 저장하지 않고 나가겠냐고 메시지바에 출력된다.
	2. 저장된 상태에서 Ctrl+q를 누르면 한 번 더 묻지 않고 나간다.
