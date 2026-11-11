<div align="center">
  <h1>
    <img src="https://raw.githubusercontent.com/ZhouBaoChuan049/ImageOfBlog/main/CppSeed.jpg" width="200" alt="CppMicroSeed Logo"/>
    <br/>
    CppMicroSeed
  </h1>

  <p>
    A C++17 microservice project scaffolding tool for standardized and maintainable service development.
  </p>
  <p>
    <i>Build a solid foundation. Focus on what matters.</i>
  </p>

  <p>
    <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white" alt="C++17"/>
    <img src="https://img.shields.io/badge/CMake-Build-064F8C?logo=cmake&logoColor=white" alt="CMake"/>
    <img src="https://img.shields.io/badge/gflags-CLI-4285F4" alt="gflags"/>
    <img src="https://img.shields.io/badge/spdlog-Logging-333333" alt="spdlog"/>
    <img src="https://img.shields.io/badge/GoogleTest-Testing-4285F4?logo=googletest&logoColor=white" alt="GoogleTest"/>
    <img src="https://img.shields.io/badge/JsonCpp-JSON-333333" alt="JsonCpp"/>
    <img src="https://img.shields.io/badge/Protocol_Buffers-Serialization-4285F4?logo=google&logoColor=white" alt="Protobuf"/>
    <img src="https://img.shields.io/badge/FFmpeg-Media-007808?logo=ffmpeg&logoColor=white" alt="FFmpeg"/>
    <img src="https://img.shields.io/badge/HTTP-httplib-2C5F2D" alt="httplib"/>
    <img src="https://img.shields.io/badge/WebSocket-Communication-333333?logo=websocket&logoColor=white" alt="WebSocket"/>
    <img src="https://img.shields.io/badge/brpc-RPC-00599C" alt="brpc"/>
    <img src="https://img.shields.io/badge/RabbitMQ-Messaging-FF6600?logo=rabbitmq&logoColor=white" alt="RabbitMQ"/>
    <img src="https://img.shields.io/badge/AMQP--CPP-Client-FF6600" alt="AMQP-CPP"/>
    <img src="https://img.shields.io/badge/etcd-Service_Discovery-419EDA" alt="etcd"/>
    <img src="https://img.shields.io/badge/etcd--api--v3-Client-419EDA" alt="etcd-api-v3"/>
    <img src="https://img.shields.io/badge/MySQL-Database-4479A1?logo=mysql&logoColor=white" alt="MySQL"/>
    <img src="https://img.shields.io/badge/ODB-ORM-6A5ACD" alt="ODB"/>
    <img src="https://img.shields.io/badge/Redis-Cache-DC382D?logo=redis&logoColor=white" alt="Redis"/>
    <img src="https://img.shields.io/badge/redis%2B%2B-Client-DC382D" alt="redis++"/>
    <img src="https://img.shields.io/badge/FastDFS-File_Storage-4A5568" alt="FastDFS"/>
    <img src="https://img.shields.io/badge/fastdfs--api-Client-4A5568" alt="fastdfs-api"/>
    <img src="https://img.shields.io/badge/Elasticsearch-Search-005571?logo=elasticsearch&logoColor=white" alt="Elasticsearch"/>
    <img src="https://img.shields.io/badge/elasticlient-Client-005571" alt="elasticlient"/>
    <img src="https://img.shields.io/badge/Aliyun-SMS-FF6A00" alt="Aliyun SMS"/>
  </p>
</div>

<p align="right">
  <a href="./README.md">简体中文</a> |
  <a href="./README_EN.md">English</a> |
  <a href="./README_JP.md">日本語</a>
</p>

## 1. プロジェクト概要

CppMicroSeed は、C++17 をベースとしたマイクロサービス開発用のプロジェクトスキャフォールドです。

C++ によるサーバーサイド開発で一般的に使用される基盤コンポーネント、インフラストラクチャクライアント、および開発に関する実践的なノウハウを統合し、今後のプロジェクト開発に再利用可能な基盤機能を提供することを目的としています。

本プロジェクトでは、各コンポーネントの機能実装だけでなく、コンポーネントのカプセル化、インターフェース設計、インフラストラクチャとの統合、実際の実行環境における機能検証にも重点を置いています。

## 2. クイックスタート

### 2.1 動作環境

本プロジェクトの動作環境は、コンテナベースの開発環境に大きく依存しています。詳細な構築手順については、[Microservices-Dev-Environment](https://github.com/ZhouBaoChuan049/Microservices-Dev-Environment) を参照してください。

- 開発環境およびインフラストラクチャサービスは、事前にビルドされた Docker イメージによって提供されます。そのため、ホストマシンに関連するミドルウェアを個別にインストールする必要はありません。
- Docker Compose を使用して、各サービスコンテナの一元的な管理およびオーケストレーションを行います。
- 初回デプロイ時には、GHCR からプロジェクトに必要な Docker イメージを取得する必要があります。

### 2.2 プロジェクトの取得

上記の手順に従って開発環境を構築した後、ワークスペースディレクトリに移動し、プロジェクトのソースコードを取得します。

```bash
git clone https://github.com/ZhouBaoChuan049/CppMicroSeed.git
cd CppMicroSeed
```

### 2.3 静的ライブラリのビルド

プロジェクトのルートディレクトリで `build` ディレクトリを作成して移動し、以下の CMake コマンドを実行します。

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

ビルドが完了すると、生成された静的ライブラリはビルドディレクトリ内の `lib/` ディレクトリに配置されます。

### 2.4 静的ライブラリとヘッダーファイルのインストール

`build` ディレクトリ内で以下のコマンドを実行します。

```bash
sudo make install
```

インストールが完了すると、静的ライブラリと公開ヘッダーファイルがそれぞれ `/usr/lib` および `/usr/include` にインストールされます。

### 2.5 静的ライブラリとヘッダーファイルのアンインストール

インストール済みの静的ライブラリとヘッダーファイルを削除する場合は、`build` ディレクトリ内で以下のコマンドを実行します。

```bash
sudo make uninstall
```

## 3. プロジェクトのディレクトリ構成

```text
CppMicroSeed/
├── Cmake/                          # CMake 設定および補助モジュール
│   ├── cmake_uninstall.cmake.in
│   ├── FindDepends.cmake
│   └── MicroSeedConfig.cmake.in
├── Design/                         # 設計ドキュメントおよび設計図
│   └── Brpc&Etcd.bmp
├── Example/                        # コンポーネントおよびサードパーティライブラリの使用例
│   ├── Demo_AmqpCpp/
│   ├── Demo_Brpc/
│   │   ├── BrpcAsyncServerAndClient/
│   │   ├── BrpcHttpServerAndClient/
│   │   └── BrpcSimpleServerAndClient/
│   ├── Demo_ElasticSearch/
│   ├── Demo_Etcd/
│   ├── Demo_FFmpeg/
│   ├── Demo_Gflags/
│   ├── Demo_Gtest/
│   ├── Demo_Httplib/
│   ├── Demo_Jsoncpp/
│   ├── Demo_MailSending/
│   ├── Demo_Protobuf/
│   ├── Demo_Spdlog/
│   └── Demo_WebSocketpp/
├── Include/                         # 外部に公開する共通ヘッダーファイル
│   ├── Brpc.h
│   ├── ElasticSearch.h
│   ├── EmailSend.h
│   ├── Etcd.h
│   ├── HLSTranscoder.h
│   ├── Log.h
│   └── Util.h
├── Source/                          # 各コンポーネントの実装コード
│   ├── Brpc.cc
│   ├── ElasticSearch.cc
│   ├── EmailSend.cc
│   ├── Etcd.cc
│   ├── HLSTranscoder.cc
│   ├── Log.cc
│   └── Util.cc
├── Test/                            # プロジェクトのテストコード
│   ├── EnvironmentTest/              # 開発環境および本番環境のテスト
│   │   ├── DevEnvironmentTest/
│   │   └── ProdEnvironmentTest/
│   ├── IntegrationTestofBrpcAndEtcd/ # brpc と etcd の統合テスト
│   ├── TestMicroBrpc/                # brpc コンポーネントのテスト
│   ├── TestMicroElasticSearch/       # Elasticsearch コンポーネントのテスト
│   ├── TestMicroEmailSend/           # メール送信コンポーネントのテスト
│   ├── TestMicroEtcd/                # etcd コンポーネントのテスト
│   ├── TestMicroFFmpeg/              # FFmpeg コンポーネントのテスト
│   ├── TestMicroLog/                 # ログコンポーネントのテスト
│   └── TestMicroUtilJsoncpp/         # JSON ユーティリティのテスト
├── CMakeLists.txt
└── README.md
```

## 4. コアコンポーネント一覧

`Include` ディレクトリと `Source` ディレクトリには、それぞれ各コンポーネントの公開インターフェースと対応する実装コードが格納されています。

本プロジェクトでは機能ごとにコンポーネントを分類し、統一されたインターフェースを通じて、サードパーティライブラリやインフラストラクチャの操作をカプセル化しています。これにより、アプリケーションコードが低レベルの実装に直接依存することを抑制します。

**現在、主に以下のコンポーネントを提供しています。**

| コンポーネント名 | 公開ヘッダー | 実装ファイル | 主な機能 |
|---|---|---|---|
| MicroSeedLog | Log.h | Log.cc | spdlog をカプセル化し、ログの初期化、設定、および出力機能を提供します。 |
| MicroSeedBrpc | Brpc.h | Brpc.cc | brpc フレームワークをカプセル化し、RPC クライアントおよびサーバーの構築を支援するコンポーネントを提供します。 |
| MicroSeedElasticSearch | ElasticSearch.h | ElasticSearch.cc | サードパーティライブラリ elasticlient をカプセル化し、Elasticsearch クライアント機能を提供します。リクエストの構築や各種 API 操作を簡素化します。 |
| MicroSeedEmailSend | EmailSend.h | EmailSend.cc | libcurl をカプセル化し、メール送信処理および関連する通信機能を提供します。 |
| MicroSeedEtcd | Etcd.h | Etcd.cc | サードパーティライブラリ etcd-cpp-apiv3 をカプセル化し、etcd のキー・バリュー操作、Lease 管理、KeepAlive、Watch などの機能を提供します。 |
| MicroSeedHLS | HLSTranscoder.h | HLSTranscoder.cc | FFmpeg をカプセル化し、動画のエンコード・デコードおよび M3U8 ファイルの処理機能を提供します。 |
| MicroSeedUtil | Util.h | Util.cc | シリアライズ・デシリアライズ、ファイル入出力、文字列分割、乱数生成などの汎用ユーティリティ機能を提供します。 |
