package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.google.gson.annotations.SerializedName;

public class Authorities {

    @SerializedName("authorityId")
    private String authorityId;

    @SerializedName("grantedDate")
    private String grantedDate;

    @SerializedName("authorityType")
    private String authorityType;

    @SerializedName("authority")
    private String authority;

    public String getAuthorityId() {
        return authorityId;
    }

    public void setAuthorityId(String authorityId) {
        this.authorityId = authorityId;
    }

    public String getGrantedDate() {
        return grantedDate;
    }

    public void setGrantedDate(String grantedDate) {
        this.grantedDate = grantedDate;
    }

    public String getAuthorityType() {
        return authorityType;
    }

    public void setAuthorityType(String authorityType) {
        this.authorityType = authorityType;
    }

    public String getAuthority() {
        return authority;
    }

    public void setAuthority(String authority) {
        this.authority = authority;
    }
}
