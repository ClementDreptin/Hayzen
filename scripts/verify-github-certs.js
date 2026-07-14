const tls = require("node:tls");

const GITHUB_API_DOMAIN = "api.github.com";
const GITHUB_CDN_DOMAIN = "release-assets.githubusercontent.com";
const SUPPORTED_CERT_FINGERPRINTS = [
    "D1:CB:CA:5D:B2:D5:2A:7F:69:3B:67:4D:E5:F0:5A:1D:0C:95:7D:F0", // USERTrust ECC Certification Authority
    "2B:8F:1B:57:33:0D:BB:A2:D0:7A:6C:51:F7:0E:E9:0D:DA:B9:AD:8E", // USERTrust RSA Certification Authority
    "CA:BD:2A:79:A1:07:6A:31:F2:1D:25:36:35:CB:03:9D:43:29:A5:E8", // ISRG Root X1
    "BD:B1:B9:3C:D5:97:8D:45:C6:26:14:55:F8:DB:95:C7:5A:D1:53:AF", // ISRG Root X2
];

async function main() {
    // Make sure the GitHub API root certificate is still one of the supported ones
    const githubApiRootCert = await getRootCertificate(GITHUB_API_DOMAIN);
    if (!SUPPORTED_CERT_FINGERPRINTS.includes(githubApiRootCert.fingerprint)) {
        throw new Error(
            `${GITHUB_API_DOMAIN} root certificate is not supported.`,
        );
    }

    // Make sure the GitHub CDN root certificate is still one of the supported ones
    const githubCdnRootCert = await getRootCertificate(GITHUB_CDN_DOMAIN);
    if (!SUPPORTED_CERT_FINGERPRINTS.includes(githubCdnRootCert.fingerprint)) {
        throw new Error(
            `${GITHUB_CDN_DOMAIN} root certificate is not supported.`,
        );
    }
}

main()
    .then(() => {
        console.log("Certificates are up to date.");
    })
    .catch((error) => {
        console.error(`Error: ${error.message}`);
        process.exit(1);
    });

// Helper function
async function getRootCertificate(hostname) {
    return new Promise((resolve, reject) => {
        const socket = tls.connect(443, hostname, {}, () => {
            let cert = socket.getPeerCertificate(true);

            // We reached the root certificate once the issuerCertificate becomes a
            // circular reference
            while (cert.issuerCertificate != cert) {
                cert = cert.issuerCertificate;
            }

            socket.end();
            resolve(cert);
        });

        socket.on("error", reject);
    });
}
