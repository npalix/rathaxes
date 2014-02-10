% Rathaxes class to write articles (short documents) in English.
%
% Usage:
%
% \documentclass{rtxarticle}
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
\ProvidesClass{rtxarticle}[2010/11/08 Rathaxes article document class]

\global\def\@rtxlanguage{francais}
\DeclareOption{francais}{\def\@rtxlanguage{francais}}
\DeclareOption{american}{\def\@rtxlanguage{american}}

% Apply given options to the scrartcl class
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrartcl}}

\ProcessOptions

% Inherit from the scrartcl class
\LoadClass{scrartcl}

% Load rathaxes stylesheet
\input{@RATHAXES_BINARY_DIR@/maintainers/latex/rathaxes.sty}

% Add our logos in the titlepage
\def\@publishers{%
\vskip 1.5em
\includegraphics[height=1.3cm]{logo_latex}\color{rathaxesred}{\sffamily{Rathaxes}}%
}

% Define a command to generate the title block
\newcommand{\rtxmaketitleblock}{
\section*{\@titleblocksectiontitle}
\@titleblock
\section*{Contact}
\@contact
}
