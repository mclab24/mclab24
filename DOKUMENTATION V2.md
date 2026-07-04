📚 Dokumentation: Deployment-System Aufbau

| Metadata | Value |
|----------|-------|
| **Datum** | 02.07.2026 |
| **Version** | 2.0.0 |
| **Projektleiter** | Stefan (mclab24) |
| **E-Mail** | stefan@mclab24.de |
| **Repository** | https://github.com/mclab24/mclab24 |

---

## 📋 Inhaltsverzeichnis

- [🎯 Ausgangssituation](#-ausgangssituation)
- [🏗️ Infrastruktur](#️-infrastruktur)
- [✅ Phase 1: GitHub Repository](#-phase-1-github-repository)
- [✅ Phase 2: Lokale Git-Struktur](#-phase-2-lokale-git-struktur)
- [✅ Phase 3: Netcup Vorbereitung](#-phase-3-netcup-vorbereitung)
- [✅ Phase 4: Deploy-Script v1.0.0](#-phase-4-deploy-script-v100)
- [✅ Phase 5: Webhook-System](#-phase-5-webhook-system)
- [✅ Phase 6: Nextcloud Synchronisation](#-phase-6-nextcloud-synchronisation)
- [✅ Phase 7: Lokaler Apache Webserver](#-phase-7-lokaler-apache-webserver)
- [🔄 Phase 8: Multi-Repo Deploy-System v2.0.0](#-phase-8-multi-repo-deploy-system-v200)
- [🏷️ Versionierung & Changelog](#️-versionierung--changelog)
- [📊 Zusammenfassung](#-zusammenfassung)
- [📞 Kontakt & Support](#-kontakt--support)

---

## 🎯 Ausgangssituation

### Zielsetzung

Monorepo auf GitHub als Master. Automatisches Deployment via `git push → Webhook → Netcup Webspace + optional Nextcloud`.

### Wichtige Einschränkung

> ⚠️ **Keine Rücksynchronisation!** Änderungen auf Webspace/Nextcloud/lokalem Testserver werden beim nächsten Deployment überschrieben.

### Geplante Automatisierung

- GitHub Webhooks/Actions oder Cronjobs auf Netcup
- Deployment über `deploy.sh`

---

## 🏗️ Infrastruktur

### Monorepo-Struktur

```
mclab24/
├── .git/
├── .gitignore
├── README.md
└── projekte/
    ├── web-projekte/
    │   └── .gitkeep
    └── code-projekte/
        └── .gitkeep
```

### Server-Übersicht

| Server | Pfad | Zweck |
|--------|------|-------|
| **Lokale Entwicklung** | `D:\GitHub\mclab24` | Arbeit am Projekt |
| **Netcup Hosting ae93f** | `/var/www/vhosts/hosting203399.ae93f.netcup.net/github.mclab24.de/httpdocs` | GitHub Deployment |
| **Nextcloud** | `/cloud.mclab24.de/projekte` | Datei-Sync |
| **Hosting a2f42** | `co-treff.de`, `mprojektede.de` | Zusätzliche Projekte |

### Lokaler Webserver

| Komponente | Pfad |
|------------|------|
| XAMPP Installation | `D:\xampp` |
| Apache Config | `D:\xampp\apache\conf\` |
| vHosts Config | `D:\xampp\apache\conf\extra\httpd-vhosts.conf` |
| hosts-Datei | `C:\Windows\System32\drivers\etc\hosts` |

---

## ✅ Phase 1: GitHub Repository

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 01.07.2026 |

### Durchgeführt

- ✅ GitHub Repository `mclab24` erstellt (public)
- ✅ Branch `main` gesetzt
- ✅ Lokal geklont nach `D:\GitHub`
- ✅ Verzeichnisstruktur erstellt (web-projekte, code-projekte, .gitkeep)
- ✅ README.md erstellt mit Projektbeschreibung
- ✅ Initial Commit + Push durchgeführt
- ✅ Git-Konfiguration: `user.name=stefab`, `user.email=stefan@mclab24.de`

### Nächste Schritte (Phase 1)

Für neue Projekte im Monorepo:

```bash
cd D:\GitHub\mclab24\projekte\web-projekte
mkdir neues-projekt
cd neues-projekt
git init
git add .
git commit -m "Initial commit"
git push
```

---

## ✅ Phase 2: Lokale Git-Struktur

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 01.07.2026 |

### Arbeitsverzeichnis

```
D:\GitHub\mclab24\
├── .git/
├── .gitignore
├── README.md
└── projekte/
    ├── web-projekte/
    └── code-projekte/
```

### Workflow für neue Projekte

1. In den entsprechenden Unterordner wechseln
2. Neues Verzeichnis erstellen
3. Git initialisieren
4. Dateien hinzufügen und committen
5. Push durchführen → automatisches Deployment

---

## ✅ Phase 3: Netcup Vorbereitung

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 01.07.2026 |

### SSH-Verbindung

```bash
ssh stefan@hosting203399.ae93f.netcup.net
```

### Durchgeführt

- ✅ Git init + remote add origin
- ✅ `git fetch origin main`
- ✅ `git reset --hard origin/main`
- ✅ `git branch --set-upstream-to=origin/main main`
- ✅ `git status` zeigt: clean
- ✅ Verzeichnisstruktur geprüft
- ✅ Nextcloud-Verzeichnis existiert

### Zielverzeichnis auf Netcup

```
/var/www/vhosts/hosting203399.ae93f.netcup.net/github.mclab24.de/httpdocs
```

---

## ✅ Phase 4: Deploy-Script v1.0.0

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 01.07.2026 |

### deploy.sh Location

```
/var/www/vhosts/hosting203399.ae93f.netcup.net/github.mclab24.de/httpdocs/deploy.sh
```

### Durchgeführt

- ✅ `deploy.sh` erstellt
- ✅ `chmod +x deploy.sh` ausgeführt
- ✅ Manuell getestet – erfolgreich
- ✅ Log-Datei: `deploy.log`

### Script-Features

| Feature | Beschreibung |
|---------|--------------|
| `--version` | Zeigt aktuelle Version |
| `--help` | Zeigt Hilfe |
| Deployment | Git Pull + Nextcloud Sync |
| Logging | Alle Aktionen in deploy.log |

---

## ✅ Phase 5: Webhook-System

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 02.07.2026 |

### Durchgeführt

- ✅ SSH-Key/Deploy Key eingerichtet
- ✅ `webhook.php` implementiert
- ✅ GitHub Webhook konfiguriert
- ✅ End-to-End Test erfolgreich

### Test-Ablauf

```
git push → webhook.php → git pull → nextcloud sync
```

### Hinweise

| Thema | Lösung |
|-------|--------|
| `mail`-Kommando | Nicht verfügbar – entfernt |
| Kopier-Befehl | rsync statt cp verwendet |
| Nextcloud-Pfad | Korrigiert auf `/cloud.mclab24.de/projekte` |

---

## ✅ Phase 6: Nextcloud Synchronisation

| Attribut | Wert |
|----------|------|
| **Version** | v1.1.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 02.07.2026 |

### Änderungen

- ✅ Nextcloud-Pfad korrigiert auf `/cloud.mclab24.de/projekte`
- ✅ Sync via rsync implementiert
- ✅ Server-Erkennung "Netcup" ergänzt
- ✅ hostname-Fehler umgangen

---

## ✅ Phase 7: Lokaler Apache Webserver

| Attribut | Wert |
|----------|------|
| **Version** | v1.0.0 |
| **Status** | ✅ Abgeschlossen |
| **Datum** | 02.07.2026 |

### XAMPP Konfiguration

XAMPP installiert unter `D:\xampp`

### Konfiguration Schritte

1. **Virtual Hosts aktivieren** in `D:\xampp\apache\conf\httpd.conf`:
   ```apache
   Include conf/extra/httpd-vhosts.conf
   ```

2. **vHosts konfiguriert** in `D:\xampp\apache\conf\extra\httpd-vhosts.conf`:

| Domain | Document Root |
|--------|---------------|
| `mclab24.local` | `D:/GitHub/mclab24/projekte/web-projekte` |
| `co-treff.local` | `D:/GitHub/co-treff` |
| `mprojektede.local` | `D:/GitHub/mprojektede` |
| `localhost` | `D:/xampp/htdocs` (default) |

3. **hosts-Datei** angepasst (`C:\Windows\System32\drivers\etc\hosts`):
   ```
   127.0.0.1 mclab24.local
   127.0.0.1 co-treff.local
   127.0.0.1 mprojektede.local
   ```

4. **Config-Test** durchgeführt:
   ```bash
   D:\xampp\apache\bin\httpd.exe -t
   ```
   Ergebnis: `Syntax OK`

5. **Apache neu gestartet**

### Test-Ergebnisse

Alle vHosts erreichbar unter:
- http://localhost – XAMPP Dashboard
- http://mclab24.local – web-projekte
- http://co-treff.local – co-treff
- http://mprojektede.local – mprojektede

---

## 🔄 Phase 8: Multi-Repo Deploy-System v2.0.0

| Attribut | Wert |
|----------|------|
| **Version** | v2.0.0 |
| **Status** | 📝 Bereit zum Deploy |
| **Datum** | 02.07.2026 |
| **Hinweis** | Migration mclab24 ausstehend |

### Architektur-Konzept

```
Jedes Repo auf dem Webspace bekommt:
├── deploy.sh          (IDENTISCH überall – das "Programm")
├── deploy.config      (repo-spezifisch – die "Einstellungen")
├── webhook.php        (IDENTISCH überall – der "Auslöser")
├── webhook.secret     (repo-spezifisch, NICHT in Git!)
└── deploy.log         (generiert – nicht in Git!)
```

### Vorteile der Architektur

- ✅ **Ein Script für beliebig viele Repos**
- ✅ **Leicht erweiterbar** – neues Repo = 3 Dateien kopieren + Config
- ✅ **Sicherheit** – Secrets ausgelagert
- ✅ **Wartbar** – zentrale Updates möglich

---

### deploy.config Beispiele

#### Für mclab24:

```bash
# ============================================
# deploy.config - mclab24 Repository
# ============================================
REPO_NAME="mclab24"
REPO_DIR="/github.mclab24.de/httpdocs"
GIT_BRANCH="main"
CLOUD_ENABLED="true"
CLOUD_DIR="/cloud.mclab24.de/projekte"
CLOUD_SOURCE="projekte"
WEBHOOK_SECRET_FILE="/github.mclab24.de/httpdocs/webhook.secret"
SCRIPT_VERSION="2.0.0"
LOG_FILE="/github.mclab24.de/httpdocs/deploy.log"
```

#### Für co-treff:

```bash
# ============================================
# deploy.config - co-treff Repository
# ============================================
REPO_NAME="co-treff"
REPO_DIR="/co-treff.de/httpdocs"
GIT_BRANCH="main"
CLOUD_ENABLED="false"
CLOUD_DIR=""
CLOUD_SOURCE=""
WEBHOOK_SECRET_FILE="/co-treff.de/httpdocs/webhook.secret"
SCRIPT_VERSION="2.0.0"
LOG_FILE="/co-treff.de/httpdocs/deploy.log"
```

#### Für mprojektede:

```bash
# ============================================
# deploy.config - mprojektede Repository
# ============================================
REPO_NAME="mprojektede"
REPO_DIR="/httpdocs/mprojektede"
GIT_BRANCH="main"
CLOUD_ENABLED="false"
CLOUD_DIR=""
CLOUD_SOURCE=""
WEBHOOK_SECRET_FILE="/httpdocs/mprojektede/webhook.secret"
SCRIPT_VERSION="2.0.0"
LOG_FILE="/httpdocs/mprojektede/deploy.log"
```

---

### Konfigurierte Repos

| Repo | Webspace-Pfad | Nextcloud | Hosting |
|------|---------------|-----------|---------|
| **mclab24** | `/github.mclab24.de/httpdocs` | ✅ Ja | ae93f |
| **co-treff** | `/co-treff.de/httpdocs` | ❌ Nein | a2f42 |
| **mprojektede** | `/httpdocs/mprojektede` | ❌ Nein | a2f42 |

---

### Migrations-Schritte für mclab24

#### 1. Backups anlegen

```bash
cd /github.mclab24.de/httpdocs
cp deploy.sh deploy.sh.backup
cp webhook.php webhook.php.backup
```

#### 2. Neue Dateien erstellen

- `deploy.config` erstellen (siehe oben)
- `deploy.sh` auf Version 2.0.0 aktualisieren
- `webhook.php` auf Version 2.0.0 aktualisieren
- `webhook.secret` mit neuem Secret erstellen

#### 3. Berechtigungen setzen

```bash
chmod +x deploy.sh
chmod 600 webhook.secret
chmod 644 deploy.config
```

#### 4. Test durchführen

```bash
./deploy.sh
```

---

### .gitignore Erweiterung

```gitignore
# Deployment-Dateien (generiert/nicht versioniert)
deploy.log
webhook.log
webhook.secret
deploy.sh.*.backup
webhook.php.*.backup
```

---

## 🏷️ Versionierung & Changelog

### Versions-Übersicht

| Komponente | Version | Status | Datum |
|-----------|---------|--------|-------|
| **deploy.sh** | 2.0.0 | 📝 Bereit | 02.07.2026 |
| **webhook.php** | 2.0.0 | 📝 Bereit | 02.07.2026 |
| **deploy.config** | 2.0.0 | 🆕 Neu | 02.07.2026 |
| **Apache vHosts** | 1.0.0 | ✅ Läuft | 02.07.2026 |

---

### Changelog

#### 🔖 v2.0.0 (02.07.2026) – Multi-Repo-Architektur

**🎯 Hauptänderung:** Umstellung von Single-Repo auf erweiterbares Multi-Repo-System

| Kategorie | Änderung |
|-----------|----------|
| ✨ **Neu** | `deploy.config` eingeführt – repo-spezifische Einstellungen ausgelagert |
| ✨ **Neu** | **Multi-Repo-fähig** – ein Script für beliebig viele Repos |
| ✨ **Neu** | `webhook.secret` – Secret aus Code ausgelagert (Sicherheit ↑) |
| ✨ **Neu** | **Apache vHosts** – lokales Testsystem für 3 Repos (`.local` Domains) |
| ✨ **Neu** | **Nextcloud-Sync optional** – per Config aktivierbar/deaktivierbar |
| 🔧 **Verbessert** | `SCRIPT_DIR`-Erkennung via `BASH_SOURCE` (robuster Pfad) |
| 🔧 **Verbessert** | Config-Validierung vor Ausführung |
| 🔧 **Verbessert** | Sauberes rsync mit `--exclude` für Deploy-Dateien |

#### 🔖 v1.1.0 (02.07.2026) – Nextcloud-Fix

| Kategorie | Änderung |
|-----------|----------|
| 🐛 **Fix** | Nextcloud-Pfad korrigiert auf `/cloud.mclab24.de/projekte` |
| 🐛 **Fix** | Server-Erkennung "Netcup" ergänzt |
| 🐛 **Fix** | hostname-Fehler umgangen |

#### 🔖 v1.0.0 (01.07.2026) – Erste funktionierende Version

| Kategorie | Änderung |
|-----------|----------|
| ✨ **Neu** | Erstes funktionierendes Deployment-System |
| ✨ **Neu** | GitHub Repository Struktur |
| ✨ **Neu** | Basis deploy.sh Script |
| ✨ **Neu** | Webhook Integration |

---

### Roadmap

| Version | Meilenstein | Status |
|---------|-------------|--------|
| **2.0.0** | Multi-Repo-Basis | 📝 Deploy ausstehend |
| **2.1.0** | co-treff Integration | 📋 Phase 9 |
| **2.2.0** | mprojektede Integration | 📋 Phase 10 |
| **3.0.0** | Zentrales Deploy-Dashboard | 💡 Idee |

---

## 📊 Zusammenfassung

### Phasen-Status

| Phase | Beschreibung | Status |
|-------|-------------|--------|
| Phase 1 | GitHub Repository | ✅ Abgeschlossen |
| Phase 2 | Lokale Git-Struktur | ✅ Abgeschlossen |
| Phase 3 | Netcup Vorbereitung | ✅ Abgeschlossen |
| Phase 4 | Deploy-Script v1.0.0 | ✅ Abgeschlossen |
| Phase 5 | Webhook-System | ✅ Abgeschlossen |
| Phase 6 | Nextcloud Synchronisation | ✅ Abgeschlossen |
| Phase 7 | Lokaler Apache Webserver | ✅ Abgeschlossen |
| Phase 8 | Multi-Repo Deploy-System v2.0.0 | 📝 Bereit zum Deploy |

### Erreichte Ziele

- ✅ Monorepo auf GitHub als Master definiert
- ✅ Automatisches Deployment via git push konfiguriert
- ✅ Netcup Webspace als Deployment-Ziel eingerichtet
- ✅ Nextcloud Synchronisation implementiert (optional)
- ✅ Lokaler Testserver mit Virtual Hosts eingerichtet
- ✅ Multi-Repo Architektur konzipiert

### Technische Details

| Komponente | Technologie |
|-----------|-------------|
| Versionierung | Git / GitHub |
| Deployment | Bash Scripts (deploy.sh) |
| Webhooks | PHP (webhook.php) |
| Sync | rsync |
| Cloud-Speicher | Nextcloud |
| Lokaler Server | XAMPP / Apache |
| Hosting | Netcup |

### Nächste Schritte

1. **mclab24 auf v2.0.0 migrieren**
   - deploy.config erstellen
   - deploy.sh und webhook.php aktualisieren
   - webhook.secret erstellen
   - Test durchführen

2. **Phase 9: co-treff Integration**
   - Gleiche Struktur auf co-treff.de deployen
   - Config ohne Nextcloud

3. **Phase 10: mprojektede Integration**
   - Gleiche Struktur auf mprojektede.de deployen
   - Config ohne Nextcloud

---

## 📞 Kontakt & Support

| Kontakt | Details |
|---------|---------|
| **Projektleiter** | Stefan (mclab24) |
| **E-Mail** | stefan@mclab24.de |
| **Repository** | https://github.com/mclab24/mclab24 |

---

<div align="center">

**Dokumentation erstellt:** 02.07.2026  
**Version:** 2.0.0  
**Status:** ✅ Aktuell

</div>
