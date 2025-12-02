
module Asciidoctor

class ExtendedHTML5Converter < (Asciidoctor::Converter.for 'html5')
    register_for 'html5'

    # overrides go here

  def convert_document node
    br = %(<br#{slash = @void_element_slash}>)
    unless (asset_uri_scheme = node.attr 'asset-uri-scheme', 'https').empty?
      asset_uri_scheme = %(#{asset_uri_scheme}:)
    end
    cdn_base_url = %(#{asset_uri_scheme}//cdnjs.cloudflare.com/ajax/libs)
    linkcss = node.attr? 'linkcss'
    max_width_attr = (node.attr? 'max-width') ? %( style="max-width: #{node.attr 'max-width'};") : ''
    result = ['<!DOCTYPE html>']
    lang_attribute = (node.attr? 'nolang') ? '' : %( lang="#{node.attr 'lang', 'en'}")
    result << %(<html#{@xml_mode ? ' xmlns="http://www.w3.org/1999/xhtml"' : ''}#{lang_attribute}>)
    result << %(<head>
<meta charset="#{node.attr 'encoding', 'UTF-8'}"#{slash}>
<meta http-equiv="X-UA-Compatible" content="IE=edge"#{slash}>
<meta name="viewport" content="width=device-width, initial-scale=1.0"#{slash}>)
    result << %(<meta name="generator" content="Asciidoctor #{node.attr 'asciidoctor-version'}"#{slash}>) unless (reproducible = node.attr? 'reproducible')
    result << %(<meta name="application-name" content="#{node.attr 'app-name'}"#{slash}>) if node.attr? 'app-name'
    result << %(<meta name="description" content="#{node.attr 'description'}"#{slash}>) if node.attr? 'description'
    result << %(<meta name="keywords" content="#{node.attr 'keywords'}"#{slash}>) if node.attr? 'keywords'
    result << %(<meta name="author" content="#{((authors = node.sub_replacements node.attr 'authors').include? '<') ? (authors.gsub XmlSanitizeRx, '') : authors}"#{slash}>) if node.attr? 'authors'
    result << %(<meta name="copyright" content="#{node.attr 'copyright'}"#{slash}>) if node.attr? 'copyright'
    if node.attr? 'favicon'
      if (icon_href = node.attr 'favicon').empty?
        icon_href = 'favicon.ico'
        icon_type = 'image/x-icon'
      elsif (icon_ext = Helpers.extname icon_href, nil)
        icon_type = icon_ext == '.ico' ? 'image/x-icon' : %(image/#{icon_ext.slice 1, icon_ext.length})
      else
        icon_type = 'image/x-icon'
      end
      result << %(<link rel="icon" type="#{icon_type}" href="#{icon_href}"#{slash}>)
    end
    result << %(<title>#{node.doctitle sanitize: true, use_fallback: true}</title>)

    if DEFAULT_STYLESHEET_KEYS.include? node.attr 'stylesheet'
      if (webfonts = node.attr 'webfonts')
        result << %(<link rel="stylesheet" href="#{asset_uri_scheme}//fonts.googleapis.com/css?family=#{webfonts.empty? ? 'Open+Sans:300,300italic,400,400italic,600,600italic%7CNoto+Serif:400,400italic,700,700italic%7CNoto+Sans+Mono:400,700' : webfonts}"#{slash}>)
      end
      if linkcss
        result << %(<link rel="stylesheet" href="#{node.normalize_web_path DEFAULT_STYLESHEET_NAME, (node.attr 'stylesdir'), false}"#{slash}>)
      else
        result << %(<style>
#{Stylesheets.instance.primary_stylesheet_data}
</style>)
      end
    elsif node.attr? 'stylesheet'
      if linkcss
        result << %(<link rel="stylesheet" href="#{node.normalize_web_path (node.attr 'stylesheet'), (node.attr 'stylesdir')}"#{slash}>)
      else
        result << %(<style>
#{node.read_contents (node.attr 'stylesheet'), start: (node.attr 'stylesdir'), warn_on_failure: true, label: 'stylesheet'}
</style>)
      end
    end

    if node.attr? 'icons', 'font'
      if node.attr? 'iconfont-remote'
        result << %(<link rel="stylesheet" href="#{node.attr 'iconfont-cdn', %(#{cdn_base_url}/font-awesome/#{FONT_AWESOME_VERSION}/css/font-awesome.min.css)}"#{slash}>)
      else
        iconfont_stylesheet = %(#{node.attr 'iconfont-name', 'font-awesome'}.css)
        result << %(<link rel="stylesheet" href="#{node.normalize_web_path iconfont_stylesheet, (node.attr 'stylesdir'), false}"#{slash}>)
      end
    end

    if (syntax_hl = node.syntax_highlighter)
      result << (syntax_hl_docinfo_head_idx = result.size)
    end

    unless (docinfo_content = node.docinfo).empty?
      result << docinfo_content
    end

    result << '</head>'
    id_attr = node.id ? %( id="#{node.id}") : ''
    if (sectioned = node.sections?) && (node.attr? 'toc-class') && (node.attr? 'toc') && (node.attr? 'toc-placement', 'auto')
      classes = [node.doctype, (node.attr 'toc-class'), %(toc-#{node.attr 'toc-position', 'header'})]
    else
      classes = [node.doctype]
    end
    classes << node.role if node.role?
    result << %(<body#{id_attr} class="#{classes.join ' '}">)

    unless (docinfo_content = node.docinfo :header).empty?
      result << docinfo_content
    end

    unless node.noheader
      result << %(<div id="header"#{max_width_attr}>)
      if node.doctype == 'manpage'
        result << %(<h1>#{node.doctitle} Manual Page</h1>)
        if sectioned && (node.attr? 'toc') && (node.attr? 'toc-placement', 'auto')
          result << %(<div id="toc" class="#{node.attr 'toc-class', 'toc'}">
<div id="toctitle">#{node.attr 'toc-title'}</div>
#{node.converter.convert node, 'outline'}
</div>)
        end
        result << (generate_manname_section node) if node.attr? 'manpurpose'
      else
        if node.header?
          unless node.notitle
            title_parts = node.doctitle partition: true
            result << %(<h1>#{title_parts.title}</h1>)
            result << %(<h1>#{title_parts.subtitle}</h1>)
          end
          details = []
          idx = 1
          node.authors.each do |author|
            details << %(<span id="author#{idx > 1 ? idx : ''}" class="author">#{node.sub_replacements author.name}</span>#{br})
            details << %(<span id="email#{idx > 1 ? idx : ''}" class="email">#{node.sub_macros author.email}</span>#{br}) if author.email
            idx += 1
          end
          if node.attr? 'revnumber'
            details << %(<span id="revnumber">#{((node.attr 'version-label') || '').downcase} #{node.attr 'revnumber'}#{(node.attr? 'revdate') ? ',' : ''}</span>)
          end
          if node.attr? 'revdate'
            details << %(<span id="revdate">#{node.attr 'revdate'}</span>)
          end
          if node.attr? 'revremark'
            details << %(#{br}<span id="revremark">#{node.attr 'revremark'}</span>)
          end
          unless details.empty?
            result << '<div class="details">'
            result.concat details
            result << '</div>'
          end
        end

        if sectioned && (node.attr? 'toc') && (node.attr? 'toc-placement', 'auto')
          result << %(<div id="toc" class="#{node.attr 'toc-class', 'toc'}">
<div id="toctitle">#{node.attr 'toc-title'}</div>
#{node.converter.convert node, 'outline'}
</div>)
        end
      end
      result << '</div>'
    end

    result << %(<div id="content"#{max_width_attr}>
#{node.content}
</div>)

    if node.footnotes? && !(node.attr? 'nofootnotes')
      result << %(<div id="footnotes"#{max_width_attr}>
<hr#{slash}>)
      node.footnotes.each do |footnote|
        result << %(<div class="footnote" id="_footnotedef_#{footnote.index}">
<a href="#_footnoteref_#{footnote.index}">#{footnote.index}</a>. #{footnote.text}
</div>)
      end
      result << '</div>'
    end

    unless node.nofooter
      result << %(<div id="footer"#{max_width_attr}>)
      result << '<div id="footer-text">'
      result << %(#{node.attr 'version-label'} #{node.attr 'revnumber'}#{br}) if node.attr? 'revnumber'
      result << %(#{node.attr 'last-update-label'} #{node.attr 'docdatetime'}) if (node.attr? 'last-update-label') && !reproducible
      result << '</div>'
      result << '</div>'
    end

    # JavaScript (and auxiliary stylesheets) loaded at the end of body for performance reasons
    # See http://www.html5rocks.com/en/tutorials/speed/script-loading/

    if syntax_hl
      if syntax_hl.docinfo? :head
        result[syntax_hl_docinfo_head_idx] = syntax_hl.docinfo :head, node, cdn_base_url: cdn_base_url, linkcss: linkcss, self_closing_tag_slash: slash
      else
        result.delete_at syntax_hl_docinfo_head_idx
      end
      if syntax_hl.docinfo? :footer
        result << (syntax_hl.docinfo :footer, node, cdn_base_url: cdn_base_url, linkcss: linkcss, self_closing_tag_slash: slash)
      end
    end

    if node.attr? 'stem'
      eqnums_val = node.attr 'eqnums', 'none'
      eqnums_val = 'AMS' if eqnums_val.empty?
      eqnums_opt = %( equationNumbers: { autoNumber: "#{eqnums_val}" } )
      # IMPORTANT inspect calls on delimiter arrays are intentional for JavaScript compat (emulates JSON.stringify)
      result << %(<script type="text/x-mathjax-config">
MathJax.Hub.Config({
  messageStyle: "none",
  tex2jax: {
    inlineMath: [#{INLINE_MATH_DELIMITERS[:latexmath].inspect}],
    displayMath: [#{BLOCK_MATH_DELIMITERS[:latexmath].inspect}],
    ignoreClass: "nostem|nolatexmath"
  },
  asciimath2jax: {
    delimiters: [#{BLOCK_MATH_DELIMITERS[:asciimath].inspect}],
    ignoreClass: "nostem|noasciimath"
  },
  TeX: {#{eqnums_opt}}
})
MathJax.Hub.Register.StartupHook("AsciiMath Jax Ready", function () {
  MathJax.InputJax.AsciiMath.postfilterHooks.Add(function (data, node) {
    if ((node = data.script.parentNode) && (node = node.parentNode) && node.classList.contains("stemblock")) {
      data.math.root.display = "block"
    }
    return data
  })
})
</script>
<script src="#{cdn_base_url}/mathjax/#{MATHJAX_VERSION}/MathJax.js?config=TeX-MML-AM_CHTML"></script>)
    end

    unless (docinfo_content = node.docinfo :footer).empty?
      result << docinfo_content
    end

    result << '</body>'
    result << '</html>'
    result.join LF
  end
end

end
