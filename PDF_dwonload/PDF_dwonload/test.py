import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
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


# 특정 iframe 내 컨테이너의 이미지 src 찾기 및 다운로드
def fetch_and_download_images_from_iframe_container(
    url, iframe_id="docFrame", container_id="container", fnm_class="fnm", save_directory="images"
):
    # Chrome 옵션 설정
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")

    # WebDriver 초기화
    driver = webdriver.Chrome(options=chrome_options)

    image_paths = []
    file_name = "output.pdf"  # 기본 파일 이름

    try:
        # URL 열기
        driver.get(url)
        driver.implicitly_wait(10)

        # iframe 전환
        iframe = WebDriverWait(driver, 20).until(
            EC.presence_of_element_located((By.ID, iframe_id))
        )
        driver.switch_to.frame(iframe)

        # 파일 이름 설정 (class="fnm"에서 텍스트 가져오기)
        try:
            file_name_element = WebDriverWait(driver, 10).until(
                EC.presence_of_element_located((By.CLASS_NAME, fnm_class))
            )
            file_name = file_name_element.text.strip() + ".pdf"  # 확장자 추가
            print(f"File name set to: {file_name}")
        except Exception as e:
            print(f"Failed to fetch file name from class '{fnm_class}': {e}")

        # 컨테이너 요소 찾기
        container = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.ID, container_id))
        )

        # 컨테이너 내의 모든 이미지 태그 찾기
        image_elements = container.find_elements(By.TAG_NAME, "img")
        print(f"Found {len(image_elements)} images in the container with ID '{container_id}' inside iframe '{iframe_id}'.")

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

    return image_paths, file_name


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
iframe_id = "docFrame"  # iframe ID
container_id = "container"  # 컨테이너 ID
fnm_class = "fnm"  # 파일 이름을 가져올 클래스
save_directory = "images"

# 특정 iframe 내 컨테이너의 이미지 다운로드
image_paths, file_name = fetch_and_download_images_from_iframe_container(
    url, iframe_id, container_id, fnm_class, save_directory
)

# 이미지 병합 및 PDF 생성
if image_paths:
    merge_images_to_pdf(image_paths, file_name)
else:
    print("No images were downloaded.")
