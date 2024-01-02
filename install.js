const fs = require('fs');
const https = require('https');
const AdmZip = require('adm-zip');

function downloadAndUnzip(mediafireUrl, destinationFolder) {
    // Download the zip file
    https.get(mediafireUrl, (response) => {
        if (response.statusCode === 200) {
            // Save the downloaded zip file
            const zipFilename = `${destinationFolder}/downloaded_file.zip`;
            const zipFile = fs.createWriteStream(zipFilename);
            response.pipe(zipFile);

            // When the download is complete, unzip the file
            zipFile.on('finish', () => {
                zipFile.close();

                // Unzip the file
                const zip = new AdmZip(zipFilename);
                zip.extractAllTo(destinationFolder, true);

                // Remove the downloaded zip file if needed
                fs.unlinkSync(zipFilename);

                console.log(`Download and unzip completed successfully to ${destinationFolder}`);
            });
        } else {
            console.log(`Failed to download the file. Status code: ${response.statusCode}`);
        }
    }).on('error', (error) => {
        console.error(`Error during download: ${error.message}`);
    });
}

function readMediafireUrlFromPackageJson(path) {
    const jsonData = JSON.parse(fs.readFileSync(path, 'utf-8'));
    // Assuming your MediaFire URL is stored in the 'mediafireUrl' field
    const url = jsonData.libraries.all;
    return url;
}

if (require.main === module) {
    // Path to your package.json file
    const packageJsonPath = 'package.json';

    // Path where you want to save the downloaded and extracted files
    const destinationFolder = '';

    // Read MediaFire URL from package.json
    const mediafireUrl = readMediafireUrlFromPackageJson(packageJsonPath);

    // Download and unzip the file
    if (mediafireUrl) {
        downloadAndUnzip(mediafireUrl, destinationFolder);
    } else {
        console.log('MediaFire URL not found in package.json.');
    }
}
