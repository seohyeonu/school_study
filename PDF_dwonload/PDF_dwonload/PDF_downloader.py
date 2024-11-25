import requests
from bs4 import BeautifulSoup
import os

def download_image(image_url, save_path="downloaded_image.png"):
    try:
        # 이미지 데이터 가져오기
        response = requests.get(image_url)
        response.raise_for_status()  # 요청 상태 확인

        # 파일 저장
        with open(save_path, "wb") as file:
            file.write(response.content)

        print(f"Image successfully downloaded and saved as {save_path}")
    except requests.exceptions.RequestException as e:
        print(f"Failed to download the image: {e}")

# 프로그램 실행
if __name__ == "__main__":
    image_url = "https://doc.coursemos.co.kr/changwon/28999/aada3ea9eb117fe425fce3ff58001b48a2330226/aada3ea9eb117fe425fce3ff58001b48a2330226.files/1.png"
    save_path = "3.png"  # 저장할 파일 이름
    download_image(image_url, save_path)


