# 
# Copyright (C) 2016-2017 SCUT Router Term
#
# This is free software, licensed under the GNU Affero General Public License v3.
# See /COPYING for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

# Update source version and package version at the same time
PKG_NAME:=scutclient
PKG_VERSION:=2.2-20170325
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/scutclient/scutclient.git
PKG_SOURCE_VERSION:=cdb4fd579aea4adbe4fbb280d8a35fefb738af1b
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz

PKG_MAINTAINER:=Scutclient Project
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_LICENSE:=AGPL-3.0

PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/scutclient
  SECTION:=net
  CATEGORY:=Network
  TITLE:=SCUT Dr.com client
  DEPENDS:=
  URL:=
endef

define Package/scutclient/description
 Support SCUT private authentication protocol.
endef

define Package/scutclient/config
endef


TARGET_CFLAGS += \
	-ffunction-sections \
	-fdata-sections

CONFIGURE_VARS += \
	ac_cv_linux_vers=$(LINUX_VERSION) \
	ac_cv_header_libusb_1_0_libusb_h=no \
	ac_cv_netfilter_can_compile=no


CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--disable-yydebug \
	--enable-ipv6 \
	--with-build-cc="$(HOSTCC)" \
	--with-pcap=linux \
	--without-septel \
	--without-dag \
	--without-libnl \
	--disable-can

define Build/Prepare
$(call Build/Prepare/Default)
endef

define Build/Configure
$(call Build/Configure/Default)
endef

define Package/scutclient/postinst
endef

define Package/scutclient/conffiles
/etc/config/scutclient
endef


define Package/scutclient/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/scutclient.config $(1)/etc/config/scutclient
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/scutclient.init $(1)/etc/init.d/scutclient
	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface
	$(INSTALL_BIN) ./files/scutclient.hotplug $(1)/etc/hotplug.d/iface/95-scutclient
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scutclient $(1)/usr/bin

endef

$(eval $(call BuildPackage,scutclient))
