import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from PIL import Image
from io import BytesIO


# 특정 iframe 내 컨테이너의 이미지 src 찾기 및 병합된 PDF 생성
def fetch_and_merge_images_to_pdf(
    url, iframe_id="docFrame", container_id="container", fnm_class="fnm", output_pdf="output.pdf"
):
    # Chrome 옵션 설정
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")

    # WebDriver 초기화
    driver = webdriver.Chrome(options=chrome_options)

    images = []
    file_name = output_pdf  # 기본 파일 이름

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
            file_name = file_name_element.text.strip()
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

        # 이미지 다운로드 및 메모리에서 처리
        for index, img in enumerate(image_elements):
            try:
                # src 추출
                src = img.get_attribute("src")
                if src:
                    response = requests.get(src, stream=True)
                    response.raise_for_status()
                    image = Image.open(BytesIO(response.content)).convert("RGB")
                    images.append(image)
            except Exception as e:
                print(f"Error processing image {index + 1}: {e}")

        # PDF 생성
        if images:
            images[0].save(file_name, save_all=True, append_images=images[1:])
            print(f"PDF saved successfully at: {file_name}")
        else:
            print("No images to save as PDF.")

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # WebDriver 종료
        driver.quit()


# 실행 코드
url = input("Enter the URL to fetch and download images: ")
iframe_id = "docFrame"  # iframe ID
container_id = "container"  # 컨테이너 ID
fnm_class = "fnm"  # 파일 이름을 가져올 클래스

# 특정 iframe 내 컨테이너의 이미지 병합 및 PDF 생성
fetch_and_merge_images_to_pdf(url, iframe_id, container_id, fnm_class)
