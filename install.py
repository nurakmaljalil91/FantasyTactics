import json
import requests
import zipfile
import os


def download_and_unzip(mediafire_url, destination_folder):
    # Download the zip file
    response = requests.get(mediafire_url)

    # Check if the request was successful (status code 200)
    if response.status_code == 200:
        # Save the downloaded zip file
        zip_filename = os.path.join(destination_folder, "downloaded_file.zip")
        with open(zip_filename, 'wb') as zip_file:
            zip_file.write(response.content)

        # Unzip the file
        with zipfile.ZipFile(zip_filename, 'r') as zip_ref:
            zip_ref.extractall(destination_folder)

        # Remove the downloaded zip file if needed
        os.remove(zip_filename)

        print(f"Download and unzip completed successfully to {destination_folder}")
    else:
        print(f"Failed to download the file. Status code: {response.status_code}")


def read_mediafire_url_from_package_json(path):
    with open(path, 'r') as json_file:
        data = json.load(json_file)
        # Assuming your MediaFire URL is stored in the 'mediafireUrl' field
        url = data.get('libraries', {}).get('all')
        return url


if __name__ == "__main__":
    # Path to your package.json file
    package_json_path = "package.json"

    # Path where you want to save the downloaded and extracted files
    destination_folder = ""

    # Read MediaFire URL from package.json
    mediafire_url = read_mediafire_url_from_package_json(package_json_path)

    # Download and unzip the file
    if mediafire_url:
        download_and_unzip(mediafire_url, destination_folder)
    else:
        print("MediaFire URL not found in package.json.")
