// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;

Common::WStringLiteral const RepositoryCredentialsDescription::AccountNameParameter(L"username");
Common::WStringLiteral const RepositoryCredentialsDescription::PasswordParameter(L"password");
Common::WStringLiteral const RepositoryCredentialsDescription::EmailParameter(L"email");

RepositoryCredentialsDescription::RepositoryCredentialsDescription()
    : AccountName(),
    Password(),
    IsPasswordEncrypted(false),
    Email()
{
}

bool RepositoryCredentialsDescription::operator== (RepositoryCredentialsDescription const & other) const
{
    return (StringUtility::AreEqualCaseInsensitive(AccountName, other.AccountName) &&
        StringUtility::AreEqualCaseInsensitive(Password, other.Password) &&
        StringUtility::AreEqualCaseInsensitive(Email, other.Email) &&
        IsPasswordEncrypted == other.IsPasswordEncrypted);
}

bool RepositoryCredentialsDescription::operator!= (RepositoryCredentialsDescription const & other) const
{
    return !(*this == other);
}

void RepositoryCredentialsDescription::WriteTo(TextWriter & w, FormatOptions const &) const
{
    w.Write("RepositoryCredentialsDescription { ");
    w.Write("AccountName = {0}, ", AccountName);
    w.Write("Email = {0}, ", Email);
 
    w.Write("}");
}

void RepositoryCredentialsDescription::ReadFromXml(
    XmlReaderUPtr const & xmlReader)
{
    xmlReader->StartElement(
        *SchemaNames::Element_RepositoryCredentials,
        *SchemaNames::Namespace);

    this->AccountName = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_AccountName);
    this->Password = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_Password);
    if (xmlReader->HasAttribute(*SchemaNames::Attribute_Email))
    {
        this->Email = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_Email);
    }
    if (xmlReader->HasAttribute(*SchemaNames::Attribute_PasswordEncrypted))
    {
        this->IsPasswordEncrypted = xmlReader->ReadAttributeValueAs<bool>(*SchemaNames::Attribute_PasswordEncrypted);
    }
    else
    {
        this->IsPasswordEncrypted = false;
    }
    // Read the rest of the empty element
    xmlReader->ReadElement();
}

Common::ErrorCode RepositoryCredentialsDescription::WriteToXml(XmlWriterUPtr const & xmlWriter)
{	//<RepositoryCredentials>
    ErrorCode er = xmlWriter->WriteStartElement(*SchemaNames::Element_RepositoryCredentials, L"", *SchemaNames::Namespace);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_AccountName, this->AccountName);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_Password, this->Password);
    if (!er.IsSuccess())
    {
        return er;
    }

    er = xmlWriter->WriteBooleanAttribute(*SchemaNames::Attribute_PasswordEncrypted,
        this->IsPasswordEncrypted);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_Email, this->Email);
    if (!er.IsSuccess())
    {
        return er;
    }
    //</RepositoryCredentials>
    return xmlWriter->WriteEndElement();
}

void RepositoryCredentialsDescription::clear()
{
    this->AccountName.clear();
    this->Password.clear();
    this->Email.clear();
    this->IsPasswordEncrypted = false;
}

ErrorCode RepositoryCredentialsDescription::ToPublicApi(
    __in ScopedHeap & heap,
    __out FABRIC_REPOSITORY_CREDENTIAL_DESCRIPTION & fabricCredential) const
{
    fabricCredential.AccountName = heap.AddString(this->AccountName);
    fabricCredential.Password = heap.AddString(this->Password);
    fabricCredential.Email = heap.AddString(this->Email);
    fabricCredential.IsPasswordEncrypted = this->IsPasswordEncrypted;

    fabricCredential.Reserved = nullptr;

    return ErrorCode::Success();
}

