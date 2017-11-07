# Xhaust

## Keywords
<pre><code><span style="color:gold">in</span>("&lt;prompt&gt;")                             <span style="color:gray;float:right">Take string input</span>
<span style="color:orchid">nin</span>(&lt;prompt&gt;)                              <span style="color:gray;float:right">Take numeric input</span>
<span style="color:orange">exhaust</span> &lt;value&gt; { &lt;CODE&gt; }                 <span style="color:gray;float:right">Loop &lt;CODE&gt;, deplete &lt;value&gt;</span>
<span style="color:deepskyblue">out</span>(&lt;value&gt;)                               <span style="color:gray;float:right">stdout</span>
<span style="color:deeppink">chars</span>("&lt;characters&gt;")                      <span style="color:gray;float:right">creates ascii list</span>
<span style="color:aquamarine">head</span>(&lt;list&gt;)                               <span style="color:gray;float:right">Get head of list</span>
<span style="color:tomato">tail</span>(&lt;list&gt;)                               <span style="color:gray;float:right">Get tail of list</span>
</code></pre>
## Examples

Multiply x and y:
<pre><code><span style="color:gray">//Take numeric input</span>
x = <span style="color:orchid">nin</span>()
y = <span style="color:orchid">nin</span>()

z = 0
<span style="color:orange">exhaust</span> x
{
  z = x + y
} <span style="color:gray">//deplete: x = x - 1</span>
<span style="color:deepskyblue">out</span>(z)
</code></pre>


Exhausting static value:
<pre><code>c = 1

<span style="color:gray">//Compiler needs to create variable when value is static</span>
<span style="color:orange">exhaust</span> 10
{
  c = c + 1
  <span style="color:deepskyblue">out</span>(c);
}</code></pre>

99 Bottles:
<pre><code>bottles = 99

<span style="color:orange">exhaust</span> bottles
{
  <span style="color:deepskyblue">out</span>(bottles)

  <span style="color:gray">//There is no string data type. This is syntax sugar.</span>
  <span style="color:deepskyblue">out</span>(<span style="color:deeppink">chars</span>(" bottles of beer on the wall. Take 1 down pass it around\n"))
}
</code></pre>

List Stack?
<pre><code>s = <>
<span style="color:orange">exhaust</span> 10
{
  s = 3::s
  <span style="color:deepskyblue">out</span>(s);
  <span style="color:gray">//output: <3,3,3,3,3,3,3,3,3,3></span>
}</code></pre>

Exhausting lists:
<pre><code><span style="color:gray">//Build S with construction operator</span>
s = 1::2::3::4::5::<>

<span style="color:orange">exhaust</span> s
{
  <span style="color:deepskyblue">out</span>(head(s));
  <span style="color:deepskyblue">out</span>(<span style="color:deeppink">chars</span>(" "));
  <span style="color:gray">//output:1 2 3 4 5 </span>

} <span style="color:gray">//deplete: s = tail(s)</span></code></pre>

Reversing a list:
<pre><code>s = 1::2::3::4::5::<>
r = <>

<span style="color:orange">exhaust</span> s
{
  j = <>

  <span style="color:gray">//Get last elem of s</span>
  b = -1
  z = s <span style="color:gray">//deep copy</span>
  <span style="color:orange">exhaust</span> b
  {
    if (tail(z) == <>)
    {
      b = 0
    }
    else
    {
      j = head(s)::j
      z = tail(z)
    }
  }

  <span style="color:gray">//put last elem of s at beginning of r</span>
  r = head(z)::r

  <span style="color:gray">//remove last elm from s</span>
  s = j

  <span style="color:gray">//stop depletion step</span>
  s = 0::s
}
</code></pre>
