pipeline {
  agent any

  stages {
    stage('Checkout') {
      steps { checkout scm }
    }

    stage('Build') {
      steps {
        bat '''
          @echo off
          setlocal EnableExtensions

          set "VSWHERE=%ProgramFiles(x86)%\\Microsoft Visual Studio\\Installer\\vswhere.exe"
          if not exist "%VSWHERE%" (
            echo ERROR: vswhere.exe not found: %VSWHERE%
            exit /b 2
          )

          for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\\**\\Bin\\MSBuild.exe`) do set "MSBUILD=%%i"

          if "%MSBUILD%"=="" (
            echo ERROR: MSBuild.exe not found via vswhere
            exit /b 3
          )

          echo MSBuild=%MSBUILD%
          "%MSBUILD%" test_repos.vcxproj /m /p:Configuration=Debug /p:Platform=x64
        '''
      }
    }

    stage('Test') {
      steps {
        bat '''
          @echo off
          if not exist "x64\\Debug\\test_repos.exe" (
            echo ERROR: x64\\Debug\\test_repos.exe not found
            dir /s /b test_repos.exe
            exit /b 4
          )

          x64\\Debug\\test_repos.exe --gtest_output=xml:test_report.xml
          dir test_report.xml
        '''
      }
    }
  }

  post {
    always {
      junit allowEmptyResults: true, testResults: 'test_report.xml'
      archiveArtifacts artifacts: 'test_report.xml', allowEmptyArchive: true
    }
  }
}
