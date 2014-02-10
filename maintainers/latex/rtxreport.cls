% Rathaxes class to write documents in English.
%
% Usage:
%
% \documentclass{rtxreport}
%
% Available commands:
%
% \rtxdoctype{STRING}
%   Document Type, for example: "Work report"
%
% \rtxdocversion{REVISION}
%   Document Version, for example: "1.0"
%
% \rtxdocref{STRING}
%   Document Reference, for example: "work_report_my_driver_2012"
%
% \rxdocstatus{STRING}
%   Document Status, for example: "Approved by the EIP laboratory"
%
% \rtxdochistory{TABLEROWS}
%   Document History, for example: "0.1 & 11/09/2010 & Louis Opter & Initial Version \\"
%
% \rtxmaketitleblock
%   Generate the title block from the information given with the previous
%   commands.
%
% Available definitions:
%
% \rtx
%   "Rathaxes"

\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{rtxreport}[2010/11/08 Rathaxes report document class]

\global\def\@rtxlanguage{francais}
\DeclareOption{francais}{\def\@rtxlanguage{francais}}
\DeclareOption{american}{\def\@rtxlanguage{american}}

% Apply given options to the scrreprt class
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrreprt}}

\ProcessOptions

% Inherit from the scrreprt class
\LoadClass{scrreprt}

% Load rathaxes stylesheet
\input{@RATHAXES_BINARY_DIR@/maintainers/latex/rathaxes.sty}

% Add our logos in the titlepage
\def\@subject{\includegraphics[height=5cm]{logo_latex}Rathaxes}

% Define a command to generate the title block
\newcommand{\rtxmaketitleblock}{
\newpage
\chapter*{\@titleblocksectiontitle}
\@titleblock
\section*{Contact}
\@contact
}
