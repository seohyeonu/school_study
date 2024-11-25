import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from PIL import Image


# 이미지 다운로드 함수
def download_image(image_url, save_directory, filename):
    try:
        # 요청으로 이미지 다운로드
        response = requests.get(image_url, stream=True)
        response.raise_for_status()

        # 파일 저장 경로 생성
        os.makedirs(save_directory, exist_ok=True)
        file_path = os.path.join(save_directory, filename)

        # 파일 저장
        with open(file_path, "wb") as file:
            for chunk in response.iter_content(1024):
                file.write(chunk)
        print(f"Downloaded: {file_path}")
        return file_path
    except Exception as e:
        print(f"Failed to download {image_url}: {e}")
        return None


# 특정 컨테이너 내 이미지 src 찾기 및 다운로드
def fetch_and_download_images_from_contents(url, container_id="contents", save_directory="images"):
    # Chrome 옵션 설정
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")

    # WebDriver 초기화
    driver = webdriver.Chrome(options=chrome_options)

    image_paths = []

    try:
        # URL 열기
        driver.get(url)
        driver.implicitly_wait(10)

        # 특정 컨테이너 요소 찾기
        container = driver.find_element(By.ID, container_id)
        if not container:
            print(f"Container with ID '{container_id}' not found.")
            return []

        # 컨테이너 내의 모든 이미지 태그 찾기
        image_elements = container.find_elements(By.TAG_NAME, "img")
        print(f"Found {len(image_elements)} images in the container with ID '{container_id}'.")

        # 이미지 다운로드
        for index, img in enumerate(image_elements):
            try:
                # src 추출
                src = img.get_attribute("src")
                if src:
                    filename = f"image_{index + 1}.jpg"
                    file_path = download_image(src, save_directory, filename)
                    if file_path:
                        image_paths.append(file_path)
            except Exception as e:
                print(f"Error processing image {index + 1}: {e}")

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # WebDriver 종료
        driver.quit()

    return image_paths


# 이미지 파일들을 PDF로 병합
def merge_images_to_pdf(image_paths, output_pdf):
    try:
        # PIL을 사용하여 이미지 불러오기
        images = [Image.open(img_path).convert("RGB") for img_path in image_paths]

        # 첫 번째 이미지를 기준으로 PDF 생성
        if images:
            images[0].save(output_pdf, save_all=True, append_images=images[1:])
            print(f"PDF saved successfully at: {output_pdf}")
    except Exception as e:
        print(f"Failed to create PDF: {e}")


# 실행 코드
url = input("Enter the URL to fetch and download images: ")
container_id = "contents"  # id="contents"로 설정
save_directory = "images"
output_pdf = "output.pdf"

# 특정 컨테이너 내 이미지 다운로드
image_paths = fetch_and_download_images_from_contents(url, container_id, save_directory)

# 이미지 병합 및 PDF 생성
if image_paths:
    merge_images_to_pdf(image_paths, output_pdf)
else:
    print("No images were downloaded.")
