/*
 * Unit test suite for cryptnet.dll
 *
 * Copyright 2007 Juan Lang
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
#include <stdarg.h>
#include <stdio.h>
#define NONAMELESSUNION
#include <windef.h>
#include <winbase.h>
#include <winerror.h>
#include <wincrypt.h>
#include "wine/test.h"

static const BYTE bigCert[] = {
0x30,0x78,0x02,0x01,0x01,0x30,0x02,0x06,0x00,0x30,0x14,0x31,0x12,0x30,0x10,
0x06,0x03,0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,0x61,0x6e,
0x67,0x30,0x22,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x31,0x30,0x30,
0x30,0x30,0x30,0x30,0x5a,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x31,
0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x30,0x14,0x31,0x12,0x30,0x10,0x06,0x03,
0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,0x61,0x6e,0x67,0x30,
0x07,0x30,0x02,0x06,0x00,0x03,0x01,0x00,0xa3,0x16,0x30,0x14,0x30,0x12,0x06,
0x03,0x55,0x1d,0x13,0x01,0x01,0xff,0x04,0x08,0x30,0x06,0x01,0x01,0xff,0x02,
0x01,0x01};
static const BYTE certWithIssuingDistPoint[] = {
0x30,0x81,0x99,0xa0,0x03,0x02,0x01,0x02,0x02,0x01,0x01,0x30,0x0d,0x06,0x09,
0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,0x30,0x14,0x31,0x12,
0x30,0x10,0x06,0x03,0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,
0x61,0x6e,0x67,0x30,0x22,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x31,
0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,
0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x30,0x14,0x31,0x12,0x30,0x10,
0x06,0x03,0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,0x61,0x6e,
0x67,0x30,0x07,0x30,0x02,0x06,0x00,0x03,0x01,0x00,0xa3,0x27,0x30,0x25,0x30,
0x23,0x06,0x03,0x55,0x1d,0x1c,0x01,0x01,0xff,0x04,0x19,0x30,0x17,0xa0,0x15,
0xa0,0x13,0x86,0x11,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x69,0x6e,0x65,
0x68,0x71,0x2e,0x6f,0x72,0x67, };
static const BYTE certWithCRLDistPoint[] = {
0x30,0x81,0x9b,0xa0,0x03,0x02,0x01,0x02,0x02,0x01,0x01,0x30,0x0d,0x06,0x09,
0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,0x30,0x14,0x31,0x12,
0x30,0x10,0x06,0x03,0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,
0x61,0x6e,0x67,0x30,0x22,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x31,
0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x18,0x0f,0x31,0x36,0x30,0x31,0x30,0x31,
0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x30,0x14,0x31,0x12,0x30,0x10,
0x06,0x03,0x55,0x04,0x03,0x13,0x09,0x4a,0x75,0x61,0x6e,0x20,0x4c,0x61,0x6e,
0x67,0x30,0x07,0x30,0x02,0x06,0x00,0x03,0x01,0x00,0xa3,0x29,0x30,0x27,0x30,
0x25,0x06,0x03,0x55,0x1d,0x1f,0x01,0x01,0xff,0x04,0x1b,0x30,0x19,0x30,0x17,
0xa0,0x15,0xa0,0x13,0x86,0x11,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x69,
0x6e,0x65,0x68,0x71,0x2e,0x6f,0x72,0x67, };

static void compareUrlArray(const CRYPT_URL_ARRAY *expected,
 const CRYPT_URL_ARRAY *got)
{
    ok(expected->cUrl == got->cUrl, "Expected %d URLs, got %d\n",
     expected->cUrl, got->cUrl);
    if (expected->cUrl == got->cUrl)
    {
        DWORD i;

        for (i = 0; i < got->cUrl; i++)
            ok(!lstrcmpiW(expected->rgwszUrl[i], got->rgwszUrl[i]),
             "%d: unexpected URL\n", i);
    }
}

static WCHAR url[] =
 { 'h','t','t','p',':','/','/','w','i','n','e','h','q','.','o','r','g',0 };

static void test_getObjectUrl(void)
{
    BOOL ret;
    DWORD urlArraySize = 0, infoSize = 0;
    PCCERT_CONTEXT cert;

    SetLastError(0xdeadbeef);
    ret = CryptGetObjectUrl(NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL);
    ok(!ret && GetLastError() == ERROR_FILE_NOT_FOUND,
     "Expected ERROR_FILE_NOT_FOUND, got %d\n", GetLastError());
    /* Crash
    ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, NULL, 0, NULL, NULL,
     NULL, NULL, NULL);
    ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, NULL, 0, NULL, NULL,
     NULL, &infoSize, NULL);
    ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, NULL, 0, NULL,
     &urlArraySize, NULL, &infoSize, NULL);
     */
    /* A cert with no CRL dist point extension fails.. */
    cert = CertCreateCertificateContext(X509_ASN_ENCODING, bigCert,
     sizeof(bigCert));
    SetLastError(0xdeadbeef);
    ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert, 0, NULL,
     NULL, NULL, NULL, NULL);
    ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
     "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
    CertFreeCertificateContext(cert);

    cert = CertCreateCertificateContext(X509_ASN_ENCODING,
     certWithIssuingDistPoint, sizeof(certWithIssuingDistPoint));
    if (cert)
    {
        /* This cert has no AIA extension, so expect this to fail */
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert, 0,
         NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert,
         CRYPT_GET_URL_FROM_PROPERTY, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert,
         CRYPT_GET_URL_FROM_EXTENSION, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        /* It does have an issuing dist point extension, but that's not what
         * this is looking for (it wants a CRL dist points extension)
         */
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, 0, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, CRYPT_GET_URL_FROM_PROPERTY, NULL, NULL, NULL, NULL,
         NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, CRYPT_GET_URL_FROM_EXTENSION, NULL, NULL, NULL, NULL,
         NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        CertFreeCertificateContext(cert);
    }
    cert = CertCreateCertificateContext(X509_ASN_ENCODING,
     certWithCRLDistPoint, sizeof(certWithCRLDistPoint));
    if (cert)
    {
        PCRYPT_URL_ARRAY urlArray;

        /* This cert has no AIA extension, so expect this to fail */
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert, 0,
         NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert,
         CRYPT_GET_URL_FROM_PROPERTY, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_ISSUER, (void *)cert,
         CRYPT_GET_URL_FROM_EXTENSION, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        /* It does have a CRL dist points extension */
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, 0, NULL, NULL, NULL, NULL, NULL);
        ok(!ret && GetLastError() == E_INVALIDARG,
         "Expected E_INVALIDARG, got %08x\n", GetLastError());
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, 0, NULL, NULL, NULL, &infoSize, NULL);
        ok(!ret && GetLastError() == E_INVALIDARG,
         "Expected E_INVALIDARG, got %08x\n", GetLastError());
        /* Can get it without specifying the location: */
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, 0, NULL, &urlArraySize, NULL, NULL, NULL);
        ok(ret, "CryptGetObjectUrl failed: %08x\n", GetLastError());
        urlArray = HeapAlloc(GetProcessHeap(), 0, urlArraySize);
        if (urlArray)
        {
            ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
             (void *)cert, 0, urlArray, &urlArraySize, NULL, NULL, NULL);
            ok(ret, "CryptGetObjectUrl failed: %08x\n", GetLastError());
            if (ret)
            {
                LPWSTR pUrl = url;
                CRYPT_URL_ARRAY expectedUrl = { 1, &pUrl };

                compareUrlArray(&expectedUrl, urlArray);
            }
            HeapFree(GetProcessHeap(), 0, urlArray);
        }
        /* or by specifying it's an extension: */
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, CRYPT_GET_URL_FROM_EXTENSION, NULL, &urlArraySize, NULL,
         NULL, NULL);
        ok(ret, "CryptGetObjectUrl failed: %08x\n", GetLastError());
        urlArray = HeapAlloc(GetProcessHeap(), 0, urlArraySize);
        if (urlArray)
        {
            ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
             (void *)cert, CRYPT_GET_URL_FROM_EXTENSION, urlArray,
             &urlArraySize, NULL, NULL, NULL);
            ok(ret, "CryptGetObjectUrl failed: %08x\n", GetLastError());
            if (ret)
            {
                LPWSTR pUrl = url;
                CRYPT_URL_ARRAY expectedUrl = { 1, &pUrl };

                compareUrlArray(&expectedUrl, urlArray);
            }
            HeapFree(GetProcessHeap(), 0, urlArray);
        }
        /* but it isn't contained in a property: */
        SetLastError(0xdeadbeef);
        ret = CryptGetObjectUrl(URL_OID_CERTIFICATE_CRL_DIST_POINT,
         (void *)cert, CRYPT_GET_URL_FROM_PROPERTY, NULL, &urlArraySize, NULL,
         NULL, NULL);
        ok(!ret && GetLastError() == CRYPT_E_NOT_FOUND,
         "Expected CRYPT_E_NOT_FOUND, got %08x\n", GetLastError());
        CertFreeCertificateContext(cert);
    }
}

static void make_tmp_file(LPSTR path)
{
    static char curr[MAX_PATH] = { 0 };
    char temp[MAX_PATH];
    DWORD dwNumberOfBytesWritten;
    HANDLE hf;

    if (!*curr)
        GetCurrentDirectoryA(MAX_PATH, curr);
    GetTempFileNameA(curr, "net", 0, temp);
    lstrcpyA(path, temp);
    hf = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
     FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hf, certWithCRLDistPoint, sizeof(certWithCRLDistPoint),
     &dwNumberOfBytesWritten, NULL);
    CloseHandle(hf);
}

static void test_retrieveObjectByUrl(void)
{
    BOOL ret;
    char tmpfile[MAX_PATH * 2], *ptr, url[MAX_PATH + 8];
    CRYPT_BLOB_ARRAY *pBlobArray;
    PCCERT_CONTEXT cert;
    PCCRL_CONTEXT crl;
    HCERTSTORE store;
    CRYPT_RETRIEVE_AUX_INFO aux = { 0 };
    FILETIME ft = { 0 };

    SetLastError(0xdeadbeef);
    ret = CryptRetrieveObjectByUrlA(NULL, NULL, 0, 0, NULL, NULL, NULL, NULL, NULL);
    ok(!ret && (GetLastError() == ERROR_INVALID_PARAMETER ||
                GetLastError() == E_INVALIDARG),
       "got 0x%x/%u (expected ERROR_INVALID_PARAMETER or E_INVALIDARG)\n",
       GetLastError(), GetLastError());

    make_tmp_file(tmpfile);
    ptr = strchr(tmpfile, ':');
    if (ptr)
        ptr += 2; /* skip colon and first slash */
    else
        ptr = tmpfile;
    snprintf(url, sizeof(url), "file:///%s", ptr);
    do {
        ptr = strchr(url, '\\');
        if (ptr)
            *ptr = '/';
    } while (ptr);

    pBlobArray = (CRYPT_BLOB_ARRAY *)0xdeadbeef;
    ret = CryptRetrieveObjectByUrlA(url, NULL, 0, 0, (void **)&pBlobArray,
     NULL, NULL, NULL, NULL);
    if (!ret)
    {
        /* File URL support was apparently removed in Vista/Windows 2008 */
        win_skip("File URLs not supported\n");
        return;
    }
    ok(ret, "CryptRetrieveObjectByUrlA failed: %d\n", GetLastError());
    ok(pBlobArray && pBlobArray != (CRYPT_BLOB_ARRAY *)0xdeadbeef,
     "Expected a valid pointer\n");
    if (pBlobArray && pBlobArray != (CRYPT_BLOB_ARRAY *)0xdeadbeef)
    {
        ok(pBlobArray->cBlob == 1, "Expected 1 blob, got %d\n",
         pBlobArray->cBlob);
        ok(pBlobArray->rgBlob[0].cbData == sizeof(certWithCRLDistPoint),
         "Unexpected size %d\n", pBlobArray->rgBlob[0].cbData);
        CryptMemFree(pBlobArray);
    }
    cert = (PCCERT_CONTEXT)0xdeadbeef;
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE, 0, 0,
     (void **)&cert, NULL, NULL, NULL, NULL);
    ok(cert && cert != (PCCERT_CONTEXT)0xdeadbeef, "Expected a cert\n");
    if (cert && cert != (PCCERT_CONTEXT)0xdeadbeef)
        CertFreeCertificateContext(cert);
    crl = (PCCRL_CONTEXT)0xdeadbeef;
    SetLastError(0xdeadbeef);
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CRL, 0, 0, (void **)&crl,
     NULL, NULL, NULL, NULL);
    /* w2k3,XP, newer w2k: CRYPT_E_NO_MATCH, 95: OSS_DATA_ERROR */
    ok(!ret && (GetLastError() == CRYPT_E_NO_MATCH ||
                GetLastError() == CRYPT_E_ASN1_BADTAG ||
                GetLastError() == OSS_DATA_ERROR),
       "got 0x%x/%u (expected CRYPT_E_NO_MATCH or CRYPT_E_ASN1_BADTAG or "
       "OSS_DATA_ERROR)\n", GetLastError(), GetLastError());

    /* only newer versions of cryptnet do the cleanup */
    if(!ret && GetLastError() != CRYPT_E_ASN1_BADTAG &&
               GetLastError() != OSS_DATA_ERROR) {
        ok(crl == NULL, "Expected CRL to be NULL\n");
    }

    if (crl && crl != (PCCRL_CONTEXT)0xdeadbeef)
        CertFreeCRLContext(crl);
    store = (HCERTSTORE)0xdeadbeef;
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CAPI2_ANY, 0, 0,
     &store, NULL, NULL, NULL, NULL);
    ok(ret, "CryptRetrieveObjectByUrlA failed: %d\n", GetLastError());
    if (store && store != (HCERTSTORE)0xdeadbeef)
    {
        DWORD certs = 0;

        cert = NULL;
        do {
            cert = CertEnumCertificatesInStore(store, cert);
            if (cert)
                certs++;
        } while (cert);
        ok(certs == 1, "Expected 1 cert, got %d\n", certs);
        CertCloseStore(store, 0);
    }
    /* Are file URLs cached? */
    cert = (PCCERT_CONTEXT)0xdeadbeef;
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE,
     CRYPT_CACHE_ONLY_RETRIEVAL, 0, (void **)&cert, NULL, NULL, NULL, NULL);
    ok(ret, "CryptRetrieveObjectByUrlA failed: %08x\n", GetLastError());
    if (cert && cert != (PCCERT_CONTEXT)0xdeadbeef)
        CertFreeCertificateContext(cert);

    cert = (PCCERT_CONTEXT)0xdeadbeef;
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE, 0, 0,
     (void **)&cert, NULL, NULL, NULL, &aux);
    /* w2k: success, 9x: fail with E_INVALIDARG */
    ok(ret || (GetLastError() == E_INVALIDARG),
       "got %u with 0x%x/%u (expected '!=0' or '0' with E_INVALIDARG)\n",
       ret, GetLastError(), GetLastError());
    if (cert && cert != (PCCERT_CONTEXT)0xdeadbeef)
        CertFreeCertificateContext(cert);

    cert = (PCCERT_CONTEXT)0xdeadbeef;
    aux.cbSize = sizeof(aux);
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE, 0, 0,
     (void **)&cert, NULL, NULL, NULL, &aux);
    /* w2k: success, 9x: fail with E_INVALIDARG */
    ok(ret || (GetLastError() == E_INVALIDARG),
       "got %u with 0x%x/%u (expected '!=0' or '0' with E_INVALIDARG)\n",
       ret, GetLastError(), GetLastError());
    if (!ret) {
        /* no more tests useful */
        DeleteFileA(tmpfile);
        skip("no usable CertificateContext\n");
        return;
    }
    CertFreeCertificateContext(cert);

    aux.pLastSyncTime = &ft;
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE, 0, 0,
     (void **)&cert, NULL, NULL, NULL, &aux);
    ok(ret, "CryptRetrieveObjectByUrlA failed: %08x\n", GetLastError());
    CertFreeCertificateContext(cert);
    ok(ft.dwLowDateTime || ft.dwHighDateTime,
     "Expected last sync time to be set\n");
    DeleteFileA(tmpfile);
    /* Okay, after being deleted, are file URLs still cached? */
    SetLastError(0xdeadbeef);
    ret = CryptRetrieveObjectByUrlA(url, CONTEXT_OID_CERTIFICATE,
     CRYPT_CACHE_ONLY_RETRIEVAL, 0, (void **)&cert, NULL, NULL, NULL, NULL);
    ok(!ret && (GetLastError() == ERROR_FILE_NOT_FOUND ||
     GetLastError() == ERROR_PATH_NOT_FOUND),
     "Expected ERROR_FILE_NOT_FOUND or ERROR_PATH_NOT_FOUND, got %d\n",
     GetLastError());
}

static const BYTE rootWithKeySignAndCRLSign[] = {
0x30,0x82,0x01,0xdf,0x30,0x82,0x01,0x4c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,
0x5b,0xc7,0x0b,0x27,0x99,0xbb,0x2e,0x99,0x47,0x9d,0x45,0x4e,0x7c,0x1a,0xca,
0xe8,0x30,0x09,0x06,0x05,0x2b,0x0e,0x03,0x02,0x1d,0x05,0x00,0x30,0x10,0x31,
0x0e,0x30,0x0c,0x06,0x03,0x55,0x04,0x03,0x13,0x05,0x43,0x65,0x72,0x74,0x31,
0x30,0x1e,0x17,0x0d,0x30,0x37,0x30,0x31,0x30,0x31,0x30,0x30,0x30,0x30,0x30,
0x30,0x5a,0x17,0x0d,0x30,0x37,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,
0x39,0x5a,0x30,0x10,0x31,0x0e,0x30,0x0c,0x06,0x03,0x55,0x04,0x03,0x13,0x05,
0x43,0x65,0x72,0x74,0x31,0x30,0x81,0x9f,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,
0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x81,0x8d,0x00,0x30,0x81,0x89,
0x02,0x81,0x81,0x00,0xad,0x7e,0xca,0xf3,0xe5,0x99,0xc2,0x2a,0xca,0x50,0x82,
0x7c,0x2d,0xa4,0x81,0xcd,0x0d,0x0d,0x86,0xd7,0xd8,0xb2,0xde,0xc5,0xc3,0x34,
0x9e,0x07,0x78,0x08,0x11,0x12,0x2d,0x21,0x0a,0x09,0x07,0x14,0x03,0x7a,0xe7,
0x3b,0x58,0xf1,0xde,0x3e,0x01,0x25,0x93,0xab,0x8f,0xce,0x1f,0xc1,0x33,0x91,
0xfe,0x59,0xb9,0x3b,0x9e,0x95,0x12,0x89,0x8e,0xc3,0x4b,0x98,0x1b,0x99,0xc5,
0x07,0xe2,0xdf,0x15,0x4c,0x39,0x76,0x06,0xad,0xdb,0x16,0x06,0x49,0xba,0xcd,
0x0f,0x07,0xd6,0xea,0x27,0xa6,0xfe,0x3d,0x88,0xe5,0x97,0x45,0x72,0xb6,0x1c,
0xc0,0x1c,0xb1,0xa2,0x89,0xe8,0x37,0x9e,0xf6,0x2a,0xcf,0xd5,0x1f,0x2f,0x35,
0x5e,0x8f,0x3a,0x9c,0x61,0xb1,0xf1,0x6c,0xff,0x8c,0xb2,0x2f,0x02,0x03,0x01,
0x00,0x01,0xa3,0x42,0x30,0x40,0x30,0x0e,0x06,0x03,0x55,0x1d,0x0f,0x01,0x01,
0xff,0x04,0x04,0x03,0x02,0x00,0x06,0x30,0x0f,0x06,0x03,0x55,0x1d,0x13,0x01,
0x01,0xff,0x04,0x05,0x30,0x03,0x01,0x01,0xff,0x30,0x1d,0x06,0x03,0x55,0x1d,
0x0e,0x04,0x16,0x04,0x14,0x14,0x8c,0x16,0xbb,0xbe,0x70,0xa2,0x28,0x89,0xa0,
0x58,0xff,0x98,0xbd,0xa8,0x24,0x2b,0x8a,0xe9,0x9a,0x30,0x09,0x06,0x05,0x2b,
0x0e,0x03,0x02,0x1d,0x05,0x00,0x03,0x81,0x81,0x00,0x74,0xcb,0x21,0xfd,0x2d,
0x25,0xdc,0xa5,0xaa,0xa1,0x26,0xdc,0x8b,0x40,0x11,0x64,0xae,0x5c,0x71,0x3c,
0x28,0xbc,0xf9,0xb3,0xcb,0xa5,0x94,0xb2,0x8d,0x4c,0x23,0x2b,0x9b,0xde,0x2c,
0x4c,0x30,0x04,0xc6,0x88,0x10,0x2f,0x53,0xfd,0x6c,0x82,0xf1,0x13,0xfb,0xda,
0x27,0x75,0x25,0x48,0xe4,0x72,0x09,0x2a,0xee,0xb4,0x1e,0xc9,0x55,0xf5,0xf7,
0x82,0x91,0xd8,0x4b,0xe4,0x3a,0xfe,0x97,0x87,0xdf,0xfb,0x15,0x5a,0x12,0x3e,
0x12,0xe6,0xad,0x40,0x0b,0xcf,0xee,0x1a,0x44,0xe0,0x83,0xb2,0x67,0x94,0xd4,
0x2e,0x7c,0xf2,0x06,0x9d,0xb3,0x3b,0x7e,0x2f,0xda,0x25,0x66,0x7e,0xa7,0x1f,
0x45,0xd4,0xf5,0xe3,0xdf,0x2a,0xf1,0x18,0x28,0x20,0xb5,0xf8,0xf5,0x8d,0x7a,
0x2e,0x84,0xee };
static const BYTE eeCert[] = {
0x30,0x82,0x01,0x93,0x30,0x81,0xfd,0xa0,0x03,0x02,0x01,0x02,0x02,0x01,0x01,
0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,
0x30,0x10,0x31,0x0e,0x30,0x0c,0x06,0x03,0x55,0x04,0x03,0x13,0x05,0x43,0x65,
0x72,0x74,0x31,0x30,0x1e,0x17,0x0d,0x30,0x37,0x30,0x35,0x30,0x31,0x30,0x30,
0x30,0x30,0x30,0x30,0x5a,0x17,0x0d,0x30,0x37,0x31,0x30,0x30,0x31,0x30,0x30,
0x30,0x30,0x30,0x30,0x5a,0x30,0x10,0x31,0x0e,0x30,0x0c,0x06,0x03,0x55,0x04,
0x03,0x13,0x05,0x43,0x65,0x72,0x74,0x32,0x30,0x81,0x9f,0x30,0x0d,0x06,0x09,
0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x81,0x8d,0x00,
0x30,0x81,0x89,0x02,0x81,0x81,0x00,0xb8,0x52,0xda,0xc5,0x4b,0x3f,0xe5,0x33,
0x0e,0x67,0x5f,0x48,0x21,0xdc,0x7e,0xef,0x37,0x33,0xba,0xff,0xb4,0xc6,0xdc,
0xb6,0x17,0x8e,0x20,0x55,0x07,0x12,0xd2,0x7b,0x3c,0xce,0x30,0xc5,0xa7,0x48,
0x9f,0x6e,0xfe,0xb8,0xbe,0xdb,0x9f,0x9b,0x17,0x60,0x16,0xde,0xc6,0x8b,0x47,
0xd1,0x57,0x71,0x3c,0x93,0xfc,0xbd,0xec,0x44,0x32,0x3b,0xb9,0xcf,0x6b,0x05,
0x72,0xa7,0x87,0x8e,0x7e,0xd4,0x9a,0x87,0x1c,0x2f,0xb7,0x82,0x40,0xfc,0x6a,
0x80,0x83,0x68,0x28,0xce,0x84,0xf4,0x0b,0x2e,0x44,0xcb,0x53,0xac,0x85,0x85,
0xb5,0x46,0x36,0x98,0x3c,0x10,0x02,0xaa,0x02,0xbc,0x8b,0xa2,0x23,0xb2,0xd3,
0x51,0x9a,0x22,0x4a,0xe3,0xaa,0x4e,0x7c,0xda,0x38,0xcf,0x49,0x98,0x72,0xa3,
0x02,0x03,0x01,0x00,0x01,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,
0x01,0x01,0x05,0x05,0x00,0x03,0x81,0x81,0x00,0x22,0xf1,0x66,0x00,0x79,0xd2,
0xe6,0xb2,0xb2,0xf7,0x2f,0x98,0x92,0x7d,0x73,0xc3,0x6c,0x5c,0x77,0x20,0xe3,
0xbf,0x3e,0xe0,0xb3,0x5c,0x68,0xb4,0x9b,0x3a,0x41,0xae,0x94,0xa0,0x80,0x3a,
0xfe,0x5d,0x7a,0x56,0x87,0x85,0x44,0x45,0xcf,0xa6,0xd3,0x10,0xe7,0x73,0x41,
0xf2,0x7f,0x88,0x85,0x91,0x8e,0xe6,0xec,0xe2,0xce,0x08,0xbc,0xa5,0x76,0xe5,
0x4d,0x1d,0xb7,0x70,0x31,0xdd,0xc9,0x9a,0x15,0x32,0x11,0x5a,0x4e,0x62,0xc8,
0xd1,0xf8,0xec,0x46,0x39,0x5b,0xe7,0x67,0x1f,0x58,0xe8,0xa1,0xa0,0x5b,0xf7,
0x8a,0x6d,0x5f,0x91,0x18,0xd4,0x90,0x85,0xff,0x30,0xc7,0xca,0x9c,0xc6,0x92,
0xb0,0xca,0x16,0xc4,0xa4,0xc0,0xd6,0xe8,0xff,0x15,0x19,0xd1,0x30,0x61,0xf3,
0xef,0x9f };
static const BYTE rootSignedCRL[] = {
0x30,0x82,0x01,0x1d,0x30,0x81,0x87,0x02,0x01,0x01,0x30,0x0d,0x06,0x09,0x2a,
0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,0x30,0x10,0x31,0x0e,0x30,
0x0c,0x06,0x03,0x55,0x04,0x03,0x13,0x05,0x43,0x65,0x72,0x74,0x31,0x17,0x0d,
0x30,0x37,0x30,0x39,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x5a,0x17,0x0d,
0x30,0x37,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x14,
0x30,0x12,0x02,0x01,0x01,0x17,0x0d,0x30,0x37,0x30,0x39,0x30,0x31,0x30,0x30,
0x30,0x30,0x30,0x30,0x5a,0xa0,0x2d,0x30,0x2b,0x30,0x0a,0x06,0x03,0x55,0x1d,
0x14,0x04,0x03,0x02,0x01,0x01,0x30,0x1d,0x06,0x03,0x55,0x1d,0x23,0x04,0x16,
0x04,0x14,0x14,0x8c,0x16,0xbb,0xbe,0x70,0xa2,0x28,0x89,0xa0,0x58,0xff,0x98,
0xbd,0xa8,0x24,0x2b,0x8a,0xe9,0x9a,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,
0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,0x03,0x81,0x81,0x00,0x9b,0x2b,0x99,0x0d,
0x16,0x83,0x93,0x54,0x29,0x3a,0xa6,0x53,0x5d,0xf8,0xa6,0x73,0x9f,0x2a,0x45,
0x39,0x91,0xff,0x91,0x1c,0x27,0x06,0xe8,0xdb,0x72,0x3f,0x66,0x89,0x15,0x68,
0x55,0xd5,0x49,0x63,0xa6,0x00,0xe9,0x66,0x9c,0x97,0xf9,0xb3,0xb3,0x2b,0x1b,
0xc7,0x79,0x46,0xa8,0xd8,0x2b,0x78,0x27,0xa0,0x70,0x02,0x81,0xc6,0x40,0xb3,
0x76,0x32,0x65,0x4c,0xf8,0xff,0x1d,0x41,0x6e,0x16,0x09,0xa2,0x8a,0x7b,0x0c,
0xd0,0xa6,0x9b,0x61,0xa3,0x7c,0x02,0x91,0x79,0xdf,0x6a,0x5e,0x88,0x95,0x66,
0x33,0x17,0xcb,0x5a,0xd2,0xdc,0x89,0x05,0x62,0x97,0x60,0x73,0x7b,0x2c,0x1a,
0x90,0x20,0x73,0x24,0x9f,0x45,0x22,0x4b,0xc1,0x33,0xd1,0xda,0xd8,0x7e,0x1b,
0x3d,0x74,0xd6,0x3b };

BOOL (WINAPI *pCertVerifyRevocation)(DWORD, DWORD, DWORD, void **, DWORD,
 PCERT_REVOCATION_PARA, PCERT_REVOCATION_STATUS);

static void test_verifyRevocation(void)
{
    HMODULE hCryptNet = GetModuleHandleA("cryptnet.dll");
    BOOL ret;
    CERT_REVOCATION_STATUS status = { sizeof(status), 0 };
    PCCERT_CONTEXT certs[2];
    CERT_REVOCATION_PARA revPara = { sizeof(revPara), 0 };

    pCertVerifyRevocation = (void *)GetProcAddress(hCryptNet,
     "CertDllVerifyRevocation");
    if (!pCertVerifyRevocation)
    {
        win_skip("no CertDllVerifyRevocation\n");
        return;
    }
    if (0)
    {
        /* Crash */
        ret = pCertVerifyRevocation(0, 0, 0, NULL, 0, NULL, NULL);
    }
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, 0, 0, NULL, 0, NULL, &status);
    todo_wine
    ok(!ret && GetLastError() == E_INVALIDARG,
     "expected E_INVALIDARG, got %08x\n", GetLastError());
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(X509_ASN_ENCODING, 0, 0, NULL, 0, NULL,
     &status);
    todo_wine
    ok(!ret && GetLastError() == E_INVALIDARG,
     "expected E_INVALIDARG, got %08x\n", GetLastError());
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE, 0, NULL, 0,
     NULL, &status);
    todo_wine
    ok(!ret && GetLastError() == E_INVALIDARG,
     "expected E_INVALIDARG, got %08x\n", GetLastError());
    certs[0] = CertCreateCertificateContext(X509_ASN_ENCODING, bigCert,
     sizeof(bigCert));
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     1, (void **)certs, 0, NULL, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    CertFreeCertificateContext(certs[0]);
    certs[0] = CertCreateCertificateContext(X509_ASN_ENCODING,
     rootWithKeySignAndCRLSign, sizeof(rootWithKeySignAndCRLSign));
    certs[1] = CertCreateCertificateContext(X509_ASN_ENCODING,
     eeCert, sizeof(eeCert));
    /* The root cert itself can't be checked for revocation */
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     1, (void **)certs, 0, NULL, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    /* Neither can the end cert */
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     1, (void **)&certs[1], 0, NULL, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    /* Both certs together can't, either (they're not CRLs) */
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     2, (void **)certs, 0, NULL, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    /* Now add a CRL to the hCrlStore */
    revPara.hCrlStore = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0,
     CERT_STORE_CREATE_NEW_FLAG, NULL);
    CertAddEncodedCRLToStore(revPara.hCrlStore, X509_ASN_ENCODING,
     rootSignedCRL, sizeof(rootSignedCRL), CERT_STORE_ADD_ALWAYS, NULL);
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     2, (void **)certs, 0, &revPara, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    /* Specifying CERT_VERIFY_REV_CHAIN_FLAG doesn't change things either */
    SetLastError(0xdeadbeef);
    ret = pCertVerifyRevocation(0, CERT_CONTEXT_REVOCATION_TYPE,
     2, (void **)certs, CERT_VERIFY_REV_CHAIN_FLAG, &revPara, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    /* Again, specifying the issuer cert: no change */
    revPara.pIssuerCert = certs[0];
    SetLastError(0xdeadbeef);
    ret = CertVerifyRevocation(X509_ASN_ENCODING, CERT_CONTEXT_REVOCATION_TYPE,
     1, (void **)&certs[1], 0, &revPara, &status);
    ok(!ret && GetLastError() == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", GetLastError());
    ok(status.dwError == CRYPT_E_NO_REVOCATION_CHECK,
     "expected CRYPT_E_NO_REVOCATION_CHECK, got %08x\n", status.dwError);
    ok(status.dwIndex == 0, "expected index 0, got %d\n", status.dwIndex);
    CertCloseStore(revPara.hCrlStore, 0);
    CertFreeCertificateContext(certs[1]);
    CertFreeCertificateContext(certs[0]);
}

START_TEST(cryptnet)
{
    test_getObjectUrl();
    test_retrieveObjectByUrl();
    test_verifyRevocation();
}
