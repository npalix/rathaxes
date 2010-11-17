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

% Apply given options to the report class
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}

% Pass a4paper to the report class
\PassOptionsToClass{a4paper}{report}

\ProcessOptions

% Inherit from the report class
\LoadClass{report}

% Load rathaxes stylesheet
\input{@RATHAXES_BINARY_DIR@/maintainers/latex/rathaxes.sty}

% Redefine the titlepage to add logos
\renewcommand{\maketitle}{\par
  \begingroup
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%
    \long\def\@makefntext##1{\parindent 1em\noindent
            \hb@xt@1.8em{%
                \hss\@textsuperscript{\normalfont\@thefnmark}}##1}%
    \if@twocolumn
      \ifnum \col@number=\@ne
        \@maketitle
      \else
        \twocolumn[\@maketitle]%
      \fi
    \else
      \newpage
      \global\@topnum\z@   % Prevents figures from going at top of page.
      \@maketitle
    \fi
    \thispagestyle{plain}\@thanks
  \endgroup
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\def\@maketitle{%
  \newpage
  \null
  \vskip 2em%
  \begin{center}%
  \let \footnote \thanks
    {\LARGE \@title \par}%
    \vskip 1.5em%
    {\large
      \lineskip .5em%
      \begin{tabular}[t]{c}%
        \@author
      \end{tabular}\par}%
    \vskip 1em%
    {\large \@date}%
    \vskip 4cm%
    \includegraphics[height=0.5\textwidth]{logortx2}%
    \vskip 1cm%
    \includegraphics[height=1.4cm]{logo_epitech}%
  \end{center}%
  \par
  \vskip 1.5em}

% Define a command to generate the title block
\newcommand{\rtxmaketitleblock}{\newpage\chapter*{Documentation control}\@titleblock}
