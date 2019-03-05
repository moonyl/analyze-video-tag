# analyze-video-tag
html5 비디오 태그를 이용한 영상 재생을 공부한다.

mp4 파일을 재생할 수 있는 비디오 태그 관련 html 코드는, 
<p>https://developer.mozilla.org/en-US/docs/Web/API/MediaSource/addSourceBuffer</p>
을 통해서 구했다.

파일에 대한 접근은 local 자원에 대해서는 불가능하기 때문에 웹서버가 필요하며, 간단하게 사용할 수 있는 nginx를 이용했다.
<p>http://nginx.org/en/download.html</P>

nginx 의 설치 디렉토리 중 html 디렉토리에서 기본적으로 웹페이지가 접근하기 때문에 관련 코드와 mp4파일을 복사했다. 
추후에 설정 내용을 정리해서 다른 디렉토리에서 접근하는 것도 좋을 것 같다.
