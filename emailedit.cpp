#include <emailedit.h>

#include <QStringList>

EmailEdit::EmailEdit(QObject *parent) : QObject(parent)
{

}

bool EmailEdit::emailTrue(QString mail)
{
  QString mail_x = emailExtract(mail).at(0);
  if (!mail_x.isEmpty())
    return true;
  else return false;
}

QStringList EmailEdit::emailExtract(QString str)
{
  QStringList mailList;
  QString str_right, str_domain;
  int x, y, z;

  if (str.isEmpty()) mailList << "";

  x = 0;
  while (x < str.size())
  {
      if (str[x] == 64) // если '@'
      {
          z = x; y = 0;
          while (this->right(str[z]) == 0) // если символ допустим
          {
              if (y!=0)
                { // если '@' не дейтвительно
                  if (str[z] == 64 or y == 100)
                  {
                      str_right.clear(); break;
                  }
                }
              if (str[z] == 46) // если '.' то проверяем домен
              {
                  int g = 5; str_domain.clear();
                  while (g < 0)
                  {
                      if (z+g != x)
                      {
                          if (this->domain(str_domain) == true)
                          {
                              str_right.append(str_domain); break;
                          } break;
                      }
                      g--;
                  }
              }
              str_right[y] = str[z];
              y++;
              z++;
          }

          y = x-1;
          while (this->left(str[y]) == 0) // если символ допустим
          {
              if (this->left(str[y]) == 0)
              {
                  str_right.prepend(str[y]);
              }
              y--; if (y<0) {break;}
          }
          if (y==x-1) {break;}
          if (str_right!="")
          {
            mailList << str_right.toLower();
            str_right.clear();
          }
          x = z;
      }
      x++;
  }

  return mailList;
}

int EmailEdit::right(QChar simvol)
{
    int x = 1;
    if (simvol == 64) // '@'
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol == 46) // '.'
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol == 95) // '_'
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol == 45)  // '-'
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 96 and simvol < 123) // a-z
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 64 and simvol < 91)  // A-Z
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 47 and simvol < 58) // 0-9
    {
        x = 0; return x; // если ноль то символ допустим
    }
    return x;
}
bool EmailEdit::domain(QString str)
{
    QStringList domain3;
    domain3 << ".ru" << ".ac" << ".ad" << ".ae" << ".af" << ".ag" << ".ai" << ".al" << ".am" << ".an" << ".ao"
            << ".aq" << ".ar" << ".as" << ".at" << ".au" << ".aw" << ".ax" << ".az" << ".ba" << ".bb" << ".bd"
            << ".be "<< ".bf" << ".bg" << ".bh" << ".bi" << ".bj" << ".bm" << ".bn" << ".bo" << ".br" << ".bs"
            << ".bt" << ".bv" << ".bw" << ".by" << ".bz" << ".ca" << ".cc" << ".cd" << ".cf" << ".cg" << ".ch"
            << ".ci" << ".ck" << ".cl" << ".cm" << ".cn" << ".co" << ".cr" << ".cs" << ".cu" << ".cv" << ".cx"
            << ".cy" << ".cz" << ".dd" << ".de" << ".dj" << ".dk" << ".dm" << ".do" << ".dz" << ".ec" << ".ee"
            << ".eg" << ".er" << ".es" << ".et" << ".eu" << ".fi" << ".fj" << ".fk" << ".fm" << ".fo" << ".fr"
            << ".ga" << ".gb" << ".gd" << ".ge" << ".gf" << ".gg" << ".gh" << ".gi" << ".gl" << ".gm" << ".gn"
            << ".gp" << ".gq" << ".gr" << ".gs" << ".gt" << ".gu" << ".gw" << ".gy" << ".hk" << ".hm" << ".hn"
            << ".hr" << ".ht" << ".hu" << ".id" << ".ie" << ".il" << ".im" << ".in" << ".io" << ".iq" << ".ir"
            << ".is" << ".it" << ".je" << ".jm" << ".jo" << ".jp" << ".ke" << ".kg" << ".kh" << ".ki" << ".km"
            << ".kn" << ".kp" << ".kr" << ".kw" << ".ky" << ".kz" << ".la" << ".lb" << ".lc" << ".li" << ".lk"
            << ".lr" << ".ls" << ".lt" << ".lu" << ".lv" << ".ly" << ".ma" << ".mc" << ".md" << ".me" << ".mg"
            << ".mh" << ".mk" << ".ml" << ".mm" << ".mn" << ".mo" << ".mp" << ".mq" << ".mr" << ".ms" << ".mt"
            << ".mu" << ".mv" << ".mw" << ".mx" << ".my" << ".mz" << ".na" << ".nc" << ".ne" << ".nf" << ".ng"
            << ".ni" << ".nl" << ".no" << ".np" << ".nr" << ".nu" << ".nz" << ".om" << ".pa" << ".pe" << ".pf"
            << ".pg" << ".ph" << ".pk" << ".pl" << ".pm" << ".pn" << ".pr" << ".ps" << ".pt" << ".pw" << ".py"
            << ".qa" << ".re" << ".ro" << ".rs" << ".ru" << ".rw" << ".sa" << ".sb" << ".sc" << ".sd" << ".se"
            << ".sg" << ".sh" << ".si" << ".sj" << ".sk" << ".sl" << ".sm" << ".sn" << ".so" << ".sr" << ".st"
            << ".su" << ".sv" << ".sy" << ".sz" << ".tc" << ".td" << ".tf" << ".tg" << ".th" << ".tj" << ".tk"
            << ".tl" << ".tm" << ".tn" << ".to" << ".tp" << ".tr" << ".tt" << ".tv" << ".tw" << ".tz" << ".ua"
            << ".ug" << ".uk" << ".us" << ".uy" << ".uz" << ".va" << ".vc" << ".ve" << ".vg" << ".vi" << ".vn"
            << ".vu" << ".wf" << ".ws" << ".ye" << ".yt" << ".za" << ".zm" << ".zw";

    QStringList domain4;
    domain4 << ".com" << ".biz" << ".cat" << ".com" << ".edu" << ".gov"
            << ".int" << ".mil" << ".net" << ".org" << ".pro" << ".tel" << ".xxx";
    QStringList domain5;
    domain5 << ".asia" << ".coop" << ".info" << ".jobs" << ".mobi" << ".museum" << ".name"
            << ".rich" << ".site" << ".travel";
    int i=0;
    if (str.size() == 3 )
    {
        while(str.contains(domain3.at(i)) != true and domain3.at(i)!="")
        {
            return true;
        }
    }
    i = 0;
    if (str.size() == 4 )
    {
        while(str.contains(domain4.at(i)) != true and domain4.at(i)!="")
        {
            return true;
        }
    }
    i=0;
    if (str.size() == 5 )
    {
        while(str.contains(domain5.at(i)) != true and domain5.at(i)!="")
        {
            return true;
        }
    }
}
int EmailEdit::left(QChar simvol)
{
    int x = 1;

    if (simvol == 46)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol == 95)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol == 45)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 96 and simvol < 123)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 64 and simvol < 91)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    if (simvol > 47 and simvol < 58)
    {
        x = 0; return x; // если ноль то символ допустим
    }
    return x;
}
