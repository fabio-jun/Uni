package br.uel.cliente;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.CollectionUtils;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

@Controller
public class ItemCardapioController {

    private static final String SESSION_PEDIDO =
            "sessionPedido";

    @Autowired
    ItemCardapioRepository itemCardapioRepository;

    @Autowired
    RestauranteRepository restauranteRepository;

    @GetMapping("/cardapio/{id}")
    public String mostrarCardapioRestaurante(@PathVariable("id") int id, Model model) {
        List<ItemCardapio> itensCardapio = itemCardapioRepository.findByRestauranteId(id);
        Restaurante restaurante = restauranteRepository.findById(id).orElse(null);
        assert restaurante != null;
        model.addAttribute("restauranteNome", restaurante.getNome());
        model.addAttribute("itensCardapio", itensCardapio);
        model.addAttribute("restauranteId", id);
        return "cardapio";
    }

    @GetMapping("/pedido")
    public String mostrarPedido(Model model, HttpServletRequest request) {
        List<ItemPedido> itensPedido = (List<ItemPedido>) request.getSession().getAttribute(SESSION_PEDIDO);

        if (itensPedido != null) {
            for (int i = 0; i < itensPedido.size(); i++) {
                ItemPedido itemPedido = itensPedido.get(i);
                ItemCardapio itemDoPedido = itemPedido.getItem();
                ItemCardapio itemDoBanco = itemCardapioRepository.findById(itemDoPedido.getId()).orElse(null);
                if (itemDoBanco != null) {
                    itemDoPedido.setNome(itemDoBanco.getNome());
                    itemDoPedido.setDescricao(itemDoBanco.getDescricao());
                    itemDoPedido.setPreco(itemDoBanco.getPreco());
                    itemPedido.setItem(itemDoPedido);
                } else {
                    itensPedido.remove(i);
                    i--;
                }
            }
            request.getSession().setAttribute(SESSION_PEDIDO, itensPedido);
        }

        double total = calcularTotalPedido(itensPedido);

        model.addAttribute("sessionPedido", itensPedido);
        model.addAttribute("totalPedido", total);

        return "pedido";
    }

    private double calcularTotalPedido(List<ItemPedido> pedido) {
        double total = 0.0;
        if (!CollectionUtils.isEmpty(pedido)) {
            for (ItemPedido item : pedido) {
                total += item.getItem().getPreco() * item.getQuantidade();
            }
        }
        return total;
    }

    @GetMapping("/adicionar-pedido/{id}")
    public String adicionarPedido(@PathVariable("id") int id, HttpServletRequest request) {
        ItemCardapio itemCardapio = itemCardapioRepository.findById(id)
                .orElseThrow(() -> new IllegalArgumentException("O id do item é inválido: " + id));

        List<ItemPedido> pedido = (List<ItemPedido>) request.getSession().getAttribute(SESSION_PEDIDO);

        if (pedido == null) {
            pedido = new ArrayList<>();
        }

        boolean found = false;
        for (ItemPedido item : pedido) {
            if (item.getItem().getId() == id) {
                item.setQuantidade(item.getQuantidade() + 1);
                found = true;
                break;
            }
        }

        if (!found) {
            pedido.add(new ItemPedido(itemCardapio, 1));
        }

        request.getSession().setAttribute(SESSION_PEDIDO, pedido);

        return "redirect:/pedido";
    }

    @GetMapping("/diminuir-pedido/{id}")
    public String diminuirPedido(@PathVariable("id") int id, HttpServletRequest request) {
        List<ItemPedido> pedido = (List<ItemPedido>) request.getSession().getAttribute(SESSION_PEDIDO);

        if (!CollectionUtils.isEmpty(pedido)) {
            List<ItemPedido> copiaPedido = new ArrayList<>(pedido);

            for (ItemPedido item : copiaPedido) {
                if (item.getItem().getId() == id) {
                    if (item.getQuantidade() > 1) {
                        item.setQuantidade(item.getQuantidade() - 1);
                    } else {
                        pedido.remove(item);
                    }
                    break;
                }
            }
            request.getSession().setAttribute(SESSION_PEDIDO, pedido);
        }

        return "redirect:/pedido";
    }

    @GetMapping("/pedido/remover/{id}")
    public String removerPedido(@PathVariable("id") int id, HttpServletRequest request) {
        List<ItemPedido> pedido = (List<ItemPedido>) request.getSession().getAttribute(SESSION_PEDIDO);

        if (!CollectionUtils.isEmpty(pedido)) {
            List<ItemPedido> copiaPedido = new ArrayList<>(pedido);

            for (ItemPedido item : copiaPedido) {
                if (item.getItem().getId() == id) {
                    pedido.remove(item);
                    break;
                }
            }
            request.getSession().setAttribute(SESSION_PEDIDO, pedido);
        }

        return "redirect:/pedido";
    }
}