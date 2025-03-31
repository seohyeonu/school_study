import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from PIL import Image
from io import BytesIO
import time


def fetch_and_merge_images_to_pdf(url, iframe_id="docFrame"):
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("--disable-gpu")
    chrome_options.add_argument("--window-size=1920,1080")

    driver = webdriver.Chrome(options=chrome_options)
    final_images = []

    try:
        driver.get(url)
        driver.implicitly_wait(10)

        iframe = WebDriverWait(driver, 20).until(
            EC.presence_of_element_located((By.ID, iframe_id))
        )
        driver.switch_to.frame(iframe)

        # fnm 클래스에서 파일 이름 가져오기
        file_name_element = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.CLASS_NAME, "fnm"))
        )
        file_name = file_name_element.text.strip()
        print(f"파일 이름: {file_name}")

        contents = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.CLASS_NAME, "contents_pdf"))
        )

        page_index = 0
        while True:
            try:
                page_area = WebDriverWait(driver, 5).until(
                    EC.presence_of_element_located((By.ID, f"page-area{page_index}"))
                )
                driver.execute_script("arguments[0].scrollIntoView(true);", page_area)
                time.sleep(1)

                img = WebDriverWait(driver, 10).until(
                    EC.presence_of_element_located((By.CSS_SELECTOR, f"#page-area{page_index} img"))
                )

                retry_count = 0
                while retry_count < 3:
                    src = img.get_attribute("src")
                    if src and "img_loading.png" not in src:
                        print(f"페이지 {page_index + 1} 이미지 다운로드 중...")
                        response = requests.get(src, stream=True)
                        response.raise_for_status()
                        image = Image.open(BytesIO(response.content)).convert("RGB")
                        final_images.append(image)
                        break
                    retry_count += 1
                    time.sleep(1)

                page_index += 1

            except Exception as e:
                print(f"페이지 {page_index + 1} 처리 중 종료: {e}")
                break

        if final_images:
            final_images[0].save(file_name, save_all=True, append_images=final_images[1:])
            print(f"총 {len(final_images)}장의 이미지를 PDF로 저장했습니다: {file_name}")
        else:
            print("저장할 이미지가 없습니다.")

    except Exception as e:
        print(f"오류 발생: {e}")
    finally:
        driver.quit()


# 실행 코드
url = input("URL을 입력하세요: ")
fetch_and_merge_images_to_pdf(url)
