const tls = require("node:tls");

const GITHUB_DOMAIN = "github.com";
const GITHUB_CDN_DOMAIN = "release-assets.githubusercontent.com";
const GITHUB_ISSUER_CERT_FINGERPRINT =
    "56:63:48:E6:56:E1:FA:6C:4E:2E:E6:72:59:16:E8:F4:E3:73:8A:07";
const GITHUB_CDN_ISSUER_CERT_FINGERPRINT =
    "EE:44:78:28:35:D8:FB:31:39:4D:A5:CE:B8:40:DC:93:84:9B:CF:5A";

async function main() {
    // Make sure the GitHub issuer certificate hasn't changed
    const githubIssuerCert = await getIssuerCertificate(GITHUB_DOMAIN);
    if (githubIssuerCert.fingerprint != GITHUB_ISSUER_CERT_FINGERPRINT) {
        throw new Error(`${GITHUB_DOMAIN} certificate issuer changed.`);
    }

    // Make sure the GitHub CDN issuer certificate hasn't changed
    const githubCdnCert = await getIssuerCertificate(GITHUB_CDN_DOMAIN);
    if (githubCdnCert.fingerprint != GITHUB_CDN_ISSUER_CERT_FINGERPRINT) {
        throw new Error(`${GITHUB_CDN_DOMAIN} certificate issuer changed.`);
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
async function getIssuerCertificate(hostname) {
    return new Promise((resolve, reject) => {
        const socket = tls.connect(443, hostname, {}, () => {
            const cert = socket.getPeerCertificate(true);
            socket.end();
            resolve(cert.issuerCertificate);
        });

        socket.on("error", reject);
    });
}
